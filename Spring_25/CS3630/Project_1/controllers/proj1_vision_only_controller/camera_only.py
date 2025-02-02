import numpy as np
import math
from detect_circle import detect_circle

# Instructions:
# Review the `detect_circle` function to infer and detect a circle in the image and compute its angle.

def vision_only_distance_calculation(image, camera_fov, object_diameter):
    """
    This function performs object detection and calculates the depth and angle of the detected circle from a camera sensor.

    Args:
        image: The input image from the camera
        camera_fov: The field of view of the camera in radians.
        object_diameter: The expected diameter of the object in meters.

    Returns:
        depth: The depth to the detected object from camera depth estimation in meters.
        angle: the angle of the detected circle in radians.
    """

    shape_info = detect_circle(image)

    if shape_info is None or not isinstance(shape_info, (list, tuple, np.ndarray)):
        return None, None

    if isinstance(shape_info, np.ndarray):
        shape_info = shape_info.flatten()

    if len(shape_info) != 3:
        return None, None

    x_center, y_center, radius = shape_info
    object_diameter_in_image = 2 * float(radius)

    image_width = image.shape[1]
    focal_length = image_width / (2 * math.tan(camera_fov / 2))

    original_depth = (object_diameter * focal_length) / object_diameter_in_image

    x_translation = 0.03
    y_translation = 0
    z_translation = 0.028

    depth = math.sqrt(original_depth**2 + x_translation**2 + y_translation**2 + z_translation**2)

    image_center_x = image_width / 2
    fin_angle = ((x_center - image_center_x) / image_width) * camera_fov

    return depth, fin_angle