from robot import Robot_Sim, PidController
from grid import Grid
from utils import rotate_point, grid_distance, find_centroid, separate_frontiers
import math
import numpy as np
import random

def get_wheel_velocities(robbie, coord):
    dx_world = coord[0] - robbie.x
    dy_world = coord[1] - robbie.y
    dx_robot, dy_robot = rotate_point(dx_world, dy_world, robbie.h)
    dist_to_coord = math.sqrt(dx_robot**2 + dy_robot**2)

    angle = math.atan2(dy_robot, dx_robot)
    threshold = 0.1
    
    linear_v = robbie.pid_controller.linear_controller((robbie.x, robbie.y, robbie.h), coord)
    w = robbie.pid_controller.angular_controller((robbie.x, robbie.y, robbie.h), coord)
    if angle < -threshold or angle > threshold:
        linear_v = 0
        
    robbie.v = linear_v
    robbie.w = w
    
    vl = (linear_v - robbie.wheel_dist / 2 * w) 
    vr = (linear_v + robbie.wheel_dist / 2 * w)

    omega_max = 6.279
    omega_l = vl/robbie.wheel_r
    omega_r = vr/robbie.wheel_r
    omega_current_max = max(abs(omega_l), abs(omega_r))
    if omega_current_max > omega_max:
        scale = omega_max/omega_current_max
        vl = vl*scale
        vr = vr*scale

    return vr, vl


def get_neighbors(cell):

    return [
        (cell[0]+1, cell[1]),
        (cell[0]-1, cell[1]),
        (cell[0], cell[1]+1),
        (cell[0], cell[1]-1)
    ]


def frontier_planning(robbie: Robot_Sim, grid: Grid):

    cells = []

    for cell in robbie.explored_cells:
        if grid.is_free(cell[0], cell[1]):
            neighbors = get_neighbors(cell)
            
            for neighbor in neighbors:
                if grid.is_in(neighbor[0], neighbor[1]):
                    if grid.is_free(neighbor[0], neighbor[1]):
                        if neighbor not in robbie.explored_cells:
                            if neighbor not in cells:
                                cells.append(neighbor)
    
    if not cells:
        robbie.next_coord = (robbie.x, robbie.y)
        return robbie, robbie.next_coord
                    
    sep_frontiers = separate_frontiers(cells, grid)
    
    if not sep_frontiers:
        robbie.next_coord = (robbie.x, robbie.y)
        return robbie, robbie.next_coord

    front_centroids = []
    for frontier in sep_frontiers:
        centroid = find_centroid(frontier)
        front_centroids.append((centroid, len(frontier)))

    centroid_utilities = []
    for centroid, size in front_centroids:
        dist = grid_distance(robbie.x, robbie.y, centroid[0], centroid[1])

        al1 = 1.0
        al2 = 2.0
        
        utility = al1 * dist - al2 * size
        centroid_utilities.append((centroid, utility))

    centroid_utilities.sort(key=lambda x: x[1])
    
    top_centroid = None
    for centroid, _ in centroid_utilities:
        if (centroid[0] != robbie.x or centroid[1] != robbie.y):
            if grid.is_free(centroid[0], centroid[1]):
                top_centroid = centroid
                break
    
    if top_centroid is None and cells:
        rand_index = random.randint(0, len(cells) - 1)
        top_centroid = cells[rand_index]
    
    robbie.next_coord = top_centroid

    grid.centroid = robbie.next_coord
    return robbie, robbie.next_coord


def exploration_state_machine(robbie, grid):

    linear_kp = 1.0
    linear_ki = 0.0
    linear_kd = 0.0
    angular_kp = 5.0
    angular_ki = 0.0
    angular_kd = 0.0

    if not hasattr(robbie, 'pid_controller'):
        robbie.pid_controller = PidController(linear_kp, linear_ki, linear_kd, angular_kp, angular_ki, angular_kd)
    use_rrt = False

    free_cells = robbie.get_free_cells_in_fov(grid)

    need_new_dest = False
    
    if robbie.next_coord is None:
        need_new_dest = True
    elif grid_distance(robbie.x, robbie.y, robbie.next_coord[0], robbie.next_coord[1]) < 0.5:
        need_new_dest = True
    elif not grid.is_free(int(robbie.next_coord[0]), int(robbie.next_coord[1])):
        need_new_dest = True
    
    if need_new_dest:
        robbie, robbie.next_coord = frontier_planning(robbie, grid)
    
    if grid.is_collision_with_obstacles((robbie.x, robbie.y), robbie.next_coord):
        dist = grid.rrt((robbie.x, robbie.y), robbie.next_coord)
        
        if dist and len(dist) > 1:
            robbie.path = dist
            robbie.next_coord = (dist[1].x, dist[1].y)
 
    r, l = get_wheel_velocities(robbie, robbie.next_coord)
    robbie.vr = r
    robbie.vl = l

    return robbie