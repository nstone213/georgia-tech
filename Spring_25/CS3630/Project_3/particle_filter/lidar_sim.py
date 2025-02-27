# Nicholas Stone

import math
import numpy as np
from utils import *
from wall import Wall

class LidarSim:
    # Constructor
    def __init__(self, walls: list[Wall], max_range: float, n_rays: float):
        self.walls = walls
        self.max_range = max_range
        self.n_rays = n_rays
        self.resolution = int(360 / n_rays)
        self.measurements = math.inf * np.ones(self.n_rays)

    # Simulate the lidar sensor reading
    def read(self, pose: SE2) -> np.ndarray:
        '''
        Simulate the lidar sensor readings given the current pose of the robot.

        Parameters:
        pose (SE2): The current pose of the robot, represented as an SE2 object, 
        which includes the x and y coordinates and the heading (orientation) of 
        the robot.

        Returns:
        np.ndarray: An array of simulated lidar measurements, where each element 
        represents the distance to the nearest wall for a specific lidar ray.

        Steps:
        1. Iterate through each lidar ray:
           - For each ray, calculate the angle based on the robot's heading and 
           the resolution of the lidar.
           - Determine the endpoint of the lidar ray based on the maximum range 
           and the calculated angle.

        2. Check for intersections with walls:
           - For each wall, check if the lidar ray intersects with the wall 
           using the line_rectangle_intersect function.
           - If an intersection is detected, calculate the intersection points 
           between the lidar ray and the edges of the wall.
           - Calculate the distances from the robot to these intersection points.

        4. Find the minimum distance:
           - Among all intersection points, find the minimum distance and update 
           the measurements array for the corresponding ray.

        5. Return the measurements:
           - Return the array of simulated lidar measurements.
        '''

        self.measurements = math.inf*np.ones(self.n_rays) # Webots lidar sensor returns inf for no detection

        for i in range(self.n_rays):
            angled = pose.h + math.radians((i * self.resolution) + (self.resolution / 2))
            fin_x = pose.x + self.max_range * math.cos(angled)
            fin_y = pose.y + self.max_range * math.sin(angled)
            endpoint = Point(fin_x, fin_y)
            minimum_distance = math.inf

            for wall in self.walls:
                if line_rectangle_intersect(Point(pose.x, pose.y), endpoint, wall.pose, wall.dimensions):
                    wall_corners = [
                        wall.pose.transform_point(Point(-wall.dimensions[0] / 2, -wall.dimensions[1] / 2)),
                        wall.pose.transform_point(Point(wall.dimensions[0] / 2, -wall.dimensions[1] / 2)),
                        wall.pose.transform_point(Point(wall.dimensions[0] / 2, wall.dimensions[1] / 2)),
                        wall.pose.transform_point(Point(-wall.dimensions[0] / 2, wall.dimensions[1] / 2)),
                    ]
                    for j in range(4):
                        point1 = wall_corners[j]
                        point2 = wall_corners[(j + 1) % 4]
                        intra = line_intersection(Point(pose.x, pose.y), endpoint, point1, point2)
                        if intra is not None:
                            fin_distance = distance_between_points(Point(pose.x, pose.y), intra)
                            if fin_distance < minimum_distance:
                                minimum_distance = fin_distance

            if minimum_distance < self.max_range:
                self.measurements[i] = minimum_distance

        return self.measurements