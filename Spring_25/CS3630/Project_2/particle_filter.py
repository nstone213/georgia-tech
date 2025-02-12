# Nicholas Stone

from grid import *
from particle import Particle
from utils import *
import setting
import numpy as np
np.random.seed(setting.RANDOM_SEED)
from itertools import product
from typing import List, Tuple
import utils

def create_random(count: int, grid: CozGrid) -> List[Particle]:
    """
    Returns a list of <count> random Particles in free space.

    Parameters:
        count: int, the number of random particles to create
        grid: a Grid, passed in to motion_update/measurement_update
            see grid.py for definition

    Returns:
        List of Particles with random coordinates in the grid's free space.
    """
    particles = []
    for particle in range(count):
        x, y = grid.random_free_place()
        particles.append(Particle(x, y))
    return particles
    

# ------------------------------------------------------------------------
def motion_update(old_particles: List[Particle], odometry_measurement: Tuple, grid: CozGrid) -> List[Particle]:
    new_particles = []

    for particle in old_particles:
        x_g, y_g, h_g = particle.xyh
        dx_r, dy_r, dh_r = odometry_measurement

        # Rotate the odometry measurement to the global frame
        dx_g, dy_g = rotate_point(dx_r, dy_r, h_g)

        # Add noise to the translational and heading updates
        new_x = x_g + dx_g + add_gaussian_noise(0, setting.ODOM_TRANS_SIGMA)
        new_y = y_g + dy_g + add_gaussian_noise(0, setting.ODOM_TRANS_SIGMA)
        new_heading = (h_g + dh_r + add_gaussian_noise(0, setting.ODOM_HEAD_SIGMA))

        # Ensure heading is within [-180, 180)
        if new_heading > 180:
            new_heading -= 360
        elif new_heading <= -180:
            new_heading += 360

        # Create a new particle with the updated coordinates and heading
        new_particle = Particle(new_x, new_y, new_heading)
        new_particles.append(new_particle)

    return new_particles

# ------------------------------------------------------------------------
def generate_marker_pairs(robot_marker_list: List[Tuple], particle_marker_list: List[Tuple]) -> List[Tuple]:
    """ Pair markers in order of closest distance

        Arguments:
        robot_marker_list -- List of markers observed by the robot: [(x1, y1, h1), (x2, y2, h2), ...]
        particle_marker_list -- List of markers observed by the particle: [(x1, y1, h1), (x2, y2, h2), ...]

        Returns: List[Tuple] of paired robot and particle markers: [((xp1, yp1, hp1), (xr1, yr1, hr1)), ((xp2, yp2, hp2), (xr2, yr2, hr2),), ...]
    """
    marker_pairs = []

    # While both lists have unpaired markers
    while len(robot_marker_list) > 0 and len(particle_marker_list) > 0:
        min_distance = float('inf')
        closest_pair = None
        closest_robot_marker = None
        closest_particle_marker = None

        # Iterate through each robot marker and find the closest particle marker
        for robot_marker in robot_marker_list:
            for particle_marker in particle_marker_list:
                # Calculate the Euclidean distance between the robot marker and particle marker
                dist = grid_distance(robot_marker[0], robot_marker[1], particle_marker[0], particle_marker[1])

                # If this is the closest pair, store it
                if dist < min_distance:
                    min_distance = dist
                    closest_pair = (robot_marker, particle_marker)
                    closest_robot_marker = robot_marker
                    closest_particle_marker = particle_marker

        # Once the closest pair is found, add it to the result and remove the markers from the lists
        marker_pairs.append(closest_pair)
        robot_marker_list.remove(closest_robot_marker)
        particle_marker_list.remove(closest_particle_marker)

    return marker_pairs

# ------------------------------------------------------------------------
# Constants from setting.py
MARKER_TRANS_SIGMA = 0.5  # translational error in inch
MARKER_HEAD_SIGMA = 5     # rotational error in degrees

def marker_likelihood(robot_marker: Tuple, particle_marker: Tuple) -> float:
    """ Calculate likelihood of reading this marker using Gaussian PDF. 

    Arguments:
    robot_marker -- Tuple (x,y,theta) of robot marker pose
    particle_marker -- Tuple (x,y,theta) of particle marker pose

    Returns: float probability
    """
    # Extract positions and headings (angles)
    robot_x, robot_y, robot_theta = robot_marker
    particle_x, particle_y, particle_theta = particle_marker
    
    # Calculate the distance between the robot marker and particle marker
    dist_between_markers = math.sqrt((robot_x - particle_x) ** 2 + (robot_y - particle_y) ** 2)
    
    # Calculate the difference in heading (angle) between the robot marker and particle marker
    angle_between_markers = abs(robot_theta - particle_theta)
    
    # Normalize the angle to be within the range [0, 180] degrees
    if angle_between_markers > 180:
        angle_between_markers = 360 - angle_between_markers
    
    # Calculate the likelihood using the Gaussian formula
    exp_part = (dist_between_markers ** 2) / (2 * MARKER_TRANS_SIGMA ** 2) + (angle_between_markers ** 2) / (2 * MARKER_HEAD_SIGMA ** 2)
    
    # Return the likelihood
    likelihood = math.exp(-exp_part)
    
    return likelihood

# ------------------------------------------------------------------------
def particle_likelihood(robot_marker_list: List[Tuple], particle_marker_list: List[Tuple]) -> float:
    """ Calculate likelihood of the particle pose being the robot's pose

    Arguments:
    robot_marker_list -- List of markers (x, y, theta) observed by the robot
    particle_marker_list -- List of markers (x, y, theta) observed by the particle

    Returns: float probability
    """
    l = 1.0  # Start with a likelihood of 1 (neutral)
    marker_pairs = generate_marker_pairs(robot_marker_list, particle_marker_list)
    
    # If there are no valid pairs, return a very low likelihood
    if len(marker_pairs) == 0:
        return 0.0
    
    # Update the particle likelihood using the likelihood of each marker pair
    for robot_marker, particle_marker in marker_pairs:
        # Calculate the likelihood for each marker pair
        marker_prob = marker_likelihood(robot_marker, particle_marker)
        
        # Multiply by the individual likelihoods for each marker pair
        l *= marker_prob
    
    return l

# ------------------------------------------------------------------------
def measurement_update(particles: List[Particle], measured_marker_list: List[Tuple], grid: CozGrid) -> List[Particle]:
    """ Particle filter measurement update
    
    Arguments:
    particles -- input list of particles representing belief \tilde{p}(x_{t} | u_{t})
    measured_marker_list -- list of robot-detected markers (rx, ry, rh)
    grid -- grid world map, which contains the marker information
    
    Returns:
    List of particles representing belief p(x_{t} | u_{t}) after measurement update
    """
    measured_particles = []
    particle_weights = []
    num_rand_particles = 25

    if len(measured_marker_list) > 0:
        for p in particles:
            x, y = p.xy
            if grid.is_in(x, y) and grid.is_free(x, y):  # Check if particle is in free space
                robot_marker_list = measured_marker_list.copy()
                particle_marker_list = p.read_markers(grid)
                
                # Calculate the likelihood of the particle pose based on the measured markers
                l = particle_likelihood(robot_marker_list, particle_marker_list)
            else:
                # If particle is not in free space, assign a very low likelihood
                l = 0.0001  # Low likelihood if particle is in an occupied or invalid space

            # Append the likelihood for each particle
            particle_weights.append(l)
    else:
        # If no measured markers, all particles have equal weight
        particle_weights = [1.0] * len(particles)
    
    # Normalize the particle weights
    total_weight = sum(particle_weights)
    if total_weight > 0:
        particle_weights = [weight / total_weight for weight in particle_weights]
    else:
        # If the total weight is 0 (very unlikely), fallback to uniform distribution
        particle_weights = [1.0 / len(particles)] * len(particles)
    
    # Create random particles if all weights are zero or too small
    if total_weight == 0:
        measured_particles.extend([random_particle(grid) for _ in range(num_rand_particles)])

    # Perform resampling based on particle weights
    resampled_particles = resample_particles(particles, particle_weights, len(particles) - num_rand_particles)
    
    # Add resampled and random particles to the final list
    measured_particles.extend(resampled_particles)

    return measured_particles

def random_particle(grid: CozGrid) -> Particle:
    """ Generate a random particle within the grid """
    x, y = random.choice(grid.get_free_cells())  # Get a random free cell in the grid
    theta = random.uniform(0, 360)  # Random orientation
    return Particle(x, y, theta)

def resample_particles(particles: List[Particle], weights: List[float], num_particles: int) -> List[Particle]:
    """ Resample particles based on their weights using roulette wheel selection """
    # Compute the cumulative distribution of particle weights
    cumulative_weights = [sum(weights[:i+1]) for i in range(len(weights))]
    
    resampled_particles = []
    for _ in range(num_particles):
        rand_val = random.random()
        for i, cumulative_weight in enumerate(cumulative_weights):
            if rand_val <= cumulative_weight:
                resampled_particles.append(particles[i])
                break
    
    return resampled_particles


