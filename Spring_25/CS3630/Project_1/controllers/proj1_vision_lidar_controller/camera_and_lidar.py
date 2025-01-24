import math
import numpy as np
from detect_circle import detect_circle

# Instructions:
# Step 1: Review the `detect_circle` function to infer and detect a circle in the image 
#         and compute its angle.
# Step 2: Explore the LiDAR data corresponding to the detected object. Investigate 
#         how to classify the object as either a sphere or a disk. You may reuse 
#         your code from `camera_only.py`.

def camera_and_lidar_calculation(image, camera_fov, object_diameter, lidar_data):
    """
    Performs object detection and classification by combining data from a camera and a LiDAR sensor.

    Args:
        image: The input image captured by the camera.
        camera_fov: The field of view of the camera in radians.
        object_diameter: The expected diameter of the object in meters.
        lidar_data: An array representing LiDAR distance data indexed by angle 
                                  in degrees, where each element corresponds to the distance 
                                  at a specific angle.

    Returns:
        lidar_distance: The distance to the detected object from the LiDAR sensor in meters.
        object_shape: A string indicating the shape of the detected object ("sphere" or "disk").
    """

    ###########################################################################
    # TODO: Student code begins
    ###########################################################################
    
    # Remove this!
    raise NotImplementedError
        
    ###########################################################################
    # Student code ends
    ###########################################################################

    return lidar_distance, object_shape
