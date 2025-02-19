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

        dx_g, dy_g = rotate_point(dx_r, dy_r, h_g)

        new_x = x_g + dx_g + add_gaussian_noise(0, setting.ODOM_TRANS_SIGMA)
        new_y = y_g + dy_g + add_gaussian_noise(0, setting.ODOM_TRANS_SIGMA)
        new_heading = (h_g + dh_r + add_gaussian_noise(0, setting.ODOM_HEAD_SIGMA))

        if new_heading > 180:
            new_heading -= 360
        elif new_heading <= -180:
            new_heading += 360

        new_particle = Particle(new_x, new_y, new_heading)
        new_particles.append(new_particle)

    return new_particles

# ------------------------------------------------------------------------
def generate_marker_pairs(robot_marker_list: List[Tuple], particle_marker_list: List[Tuple]) -> List[Tuple]:
    marker_pairs = []

    while len(robot_marker_list) > 0 and len(particle_marker_list) > 0:
        min_distance = float('inf')
        closest_pair = None
        closest_robot_marker = None
        closest_particle_marker = None

        for robot_marker in robot_marker_list:
            for particle_marker in particle_marker_list:
                dist = grid_distance(robot_marker[0], robot_marker[1], particle_marker[0], particle_marker[1])

                if dist < min_distance:
                    min_distance = dist
                    closest_pair = (robot_marker, particle_marker)
                    closest_robot_marker = robot_marker
                    closest_particle_marker = particle_marker

        marker_pairs.append(closest_pair)
        robot_marker_list.remove(closest_robot_marker)
        particle_marker_list.remove(closest_particle_marker)

    return marker_pairs

# ------------------------------------------------------------------------
MARKER_TRANS_SIGMA = 0.5
MARKER_HEAD_SIGMA = 5

def marker_likelihood(robot_marker: Tuple, particle_marker: Tuple) -> float:
    robot_x, robot_y, robot_theta = robot_marker
    particle_x, particle_y, particle_theta = particle_marker
    
    dist_between_markers = math.sqrt((robot_x - particle_x) ** 2 + (robot_y - particle_y) ** 2)
    
    angle_between_markers = abs(robot_theta - particle_theta)

    if angle_between_markers > 180:
        angle_between_markers = 360 - angle_between_markers

    exp_part = (dist_between_markers ** 2) / (2 * MARKER_TRANS_SIGMA ** 2) + (angle_between_markers ** 2) / (2 * MARKER_HEAD_SIGMA ** 2)
    
    likelihood = math.exp(-exp_part)
    
    return likelihood

# ------------------------------------------------------------------------
def particle_likelihood(robot_marker_list: List[Tuple], particle_marker_list: List[Tuple]) -> float:
    l = 1.0
    marker_pairs = generate_marker_pairs(robot_marker_list, particle_marker_list)
    
    if len(marker_pairs) == 0:
        return 0.0
    
    for robot_marker, particle_marker in marker_pairs:

        marker_prob = marker_likelihood(robot_marker, particle_marker)
        
        l *= marker_prob
    
    return l

# ------------------------------------------------------------------------
def measurement_update(particles: List[Particle], measured_marker_list: List[Tuple], grid: CozGrid) -> List[Particle]:
    measured_particles = []
    particle_weights = []
    num_rand_particles = 25

    if len(measured_marker_list) > 0:
        for p in particles:
            x, y = p.xy
            if grid.is_in(x, y) and grid.is_free(x, y):
                robot_marker_list = measured_marker_list.copy()
                particle_marker_list = p.read_markers(grid)

                l = particle_likelihood(robot_marker_list, particle_marker_list)
            else:
                l = 0.0001

            particle_weights.append(l)
    else:
        particle_weights = [1.0] * len(particles)
    

    total_weight = sum(particle_weights)

    if total_weight > 0:
        particle_weights = [weight / total_weight for weight in particle_weights]
    else:
        particle_weights = [1.0 / len(particles)] * len(particles)

    if total_weight == 0:
        measured_particles.extend([random_particle(grid) for _ in range(num_rand_particles)])

    resampled_particles = resample_particles(particles, particle_weights, len(particles) - num_rand_particles)

    measured_particles.extend(resampled_particles)

    return measured_particles

def random_particle(grid: CozGrid) -> Particle:
    x, y = random.choice(grid.get_free_cells())
    theta = random.uniform(0, 360)
    return Particle(x, y, theta)

def resample_particles(particles: List[Particle], weights: List[float], num_particles: int) -> List[Particle]:
    cumulative_weights = [sum(weights[:i+1]) for i in range(len(weights))]
    
    resampled_particles = []
    for _ in range(num_particles):
        rand_val = random.random()
        for i, cumulative_weight in enumerate(cumulative_weights):
            if rand_val <= cumulative_weight:
                resampled_particles.append(particles[i])
                break
    
    return resampled_particles