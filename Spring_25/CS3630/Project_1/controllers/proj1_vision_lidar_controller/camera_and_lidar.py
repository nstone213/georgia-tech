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

    shape = detect_circle(image)

    if shape is None or len(shape) == 0:
        return None

    x_center, y_center, radius = shape[0]

    image_width = image.shape[1]
    x_relative = (x_center - (image_width / 2)) / (image_width / 2)
    
    x_translation = x_center - (image_width / 2) + (0.03 * image_width)
    x_relative = x_translation / (image_width / 2)
    angle_degrees = x_relative * (camera_fov / 2) * (180 / math.pi)

    lidar_center_idx = int((angle_degrees / 360.0) * len(lidar_data))
    lidar_center_idx = max(0, min(len(lidar_data) - 1, lidar_center_idx))

    lidar_range = 7
    lidar_indices = [max(0, lidar_center_idx - i) for i in range(-lidar_range, lidar_range + 1)]
    lidar_readings = np.array([lidar_data[i] for i in lidar_indices if lidar_data[i] > 0])

    if len(lidar_readings) == 0:
        return None

    median_distance = np.median(lidar_readings)

    refined_distances = [d for d in lidar_readings if abs(d - median_distance) < 0.1 * median_distance]

    if len(refined_distances) == 0:
        return None

    lidar_center = np.median(refined_distances) - 0.028

    std = np.std(refined_distances)
    threshold = 0.006

    if std > threshold:
        object_shape = "sphere"
    else:
        object_shape = "disk"

    return lidar_center, object_shape