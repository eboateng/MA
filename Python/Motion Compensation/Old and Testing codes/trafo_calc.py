import numpy as np

def dh_trafo(a, d, roll, pitch, yaw):
    """
    Perform a 3D transformation with translational and rotational effects using Denavit-Hartenberg.

    Parameters:
    - a: Length of the common normal (in meters) from the old z-axis to the new z-axis.
    - d: Offset (in meters) along the old z-axis to the common normal.
    - roll: Roll angle (in radians) from the IMU.
    - pitch: Pitch angle (in radians) from the IMU.
    - yaw: Yaw angle (in radians) from the IMU.

    Returns:
    - A 4x4 transformation matrix.
    """
    transformation_matrix = np.array([
        [np.cos(yaw) * np.cos(pitch), -np.sin(yaw) * np.cos(roll) + np.cos(yaw) * np.sin(pitch) * np.sin(roll),
         np.sin(yaw) * np.sin(roll) + np.cos(yaw) * np.cos(roll) * np.sin(pitch), a * np.cos(yaw) * np.cos(pitch)],
        [np.sin(yaw) * np.cos(pitch), np.cos(yaw) * np.cos(roll) + np.sin(yaw) * np.sin(pitch) * np.sin(roll),
         -np.cos(yaw) * np.sin(roll) + np.sin(yaw) * np.cos(roll) * np.sin(pitch), a * np.sin(yaw) * np.cos(pitch)],
        [-np.sin(pitch), np.cos(pitch) * np.sin(roll), np.cos(pitch) * np.cos(roll), d + a * np.sin(pitch)],
        [0, 0, 0, 1]
    ])
    return transformation_matrix


def adjust_for_pitch_roll_motion(target_azimuth, target_elevation, roll, pitch, yaw):
    # Convert angles from degrees to radians
    phi = np.radians(roll)
    theta = np.radians(pitch)
    psi = -np.radians(yaw)  # Reversing the yaw rotation
    azimuth = np.radians(target_azimuth)
    elevation = np.radians(target_elevation)
    # Rotation matrices for reversed yaw, roll, and pitch
    Rz_inv_yaw = np.array([[np.cos(psi), np.sin(psi), 0], [-np.sin(psi), np.cos(psi), 0], [0, 0, 1]])
    Rx = np.array([[1, 0, 0], [0, np.cos(phi), -np.sin(phi)], [0, np.sin(phi), np.cos(phi)]])
    Ry = np.array([[np.cos(theta), 0, np.sin(theta)], [0, 1, 0], [-np.sin(theta), 0, np.cos(theta)]])
    # Combined rotation matrix, first counteracting yaw then applying roll and pitch
    R = Ry.dot(Rx).dot(Rz_inv_yaw)
    # Target vector in local coordinates
    V_local = np.array([np.cos(-elevation) * np.cos(-azimuth), np.cos(-elevation) * np.sin(-azimuth), np.sin(-elevation)])
    # Adjust the target vector for the rotation
    V_adjusted = np.linalg.inv(R).dot(V_local)
    # Calculate new azimuth and elevation from the adjusted vector
    new_azimuth = np.arctan2(V_adjusted[1], V_adjusted[0])
    new_elevation = np.arcsin(V_adjusted[2] / np.linalg.norm(V_adjusted))
    # Convert the new azimuth and elevation back to degrees
    new_azimuth_deg = np.degrees(new_azimuth)
    new_elevation_deg = np.degrees(new_elevation)
    return new_azimuth_deg, new_elevation_deg

def adjust_for_motion(target_azimuth, target_elevation, roll, pitch, yaw):
    # Convert angles from degrees to radians
    yaw = np.radians(yaw)
    phi = np.radians(roll)
    theta = np.radians(pitch)
    psi = np.radians(yaw)
    azimuth = np.radians(target_azimuth)
    elevation = np.radians(target_elevation)
    # Rotation matrices for roll, pitch, yaw
    Rx = np.array([[1, 0, 0], [0, np.cos(phi), -np.sin(phi)], [0, np.sin(phi), np.cos(phi)]])
    Ry = np.array([[np.cos(theta), 0, np.sin(theta)], [0, 1, 0], [-np.sin(theta), 0, np.cos(theta)]])
    Rz = np.array([[np.cos(psi), -np.sin(psi), 0], [np.sin(psi), np.cos(psi), 0], [0, 0, 1]])
    # Combined rotation matrix
    R = Rz.dot(Ry).dot(Rx)
    #print("R",R)
    # Target vector in local coordinates
    V_local = np.array([np.cos(-elevation) * np.cos(-azimuth), np.cos(-elevation) * np.sin(-azimuth), np.sin(-elevation)])
    #print("V_local:",V_local)
    # Adjust the target vector for the rotation
    V_adjusted = np.linalg.inv(R).dot(V_local)
    #print("V_adjusted",V_adjusted)
    # Calculate new azimuth and elevation from the adjusted vector
    new_azimuth = np.arctan2(V_adjusted[1], V_adjusted[0])
    new_elevation = np.arcsin(V_adjusted[2] / np.linalg.norm(V_adjusted))
    # Convert the new azimuth and elevation back to degrees
    new_azimuth_deg = np.degrees(new_azimuth)
    new_elevation_deg = np.degrees(new_elevation)
    
    return new_azimuth_deg, new_elevation_deg

def compensateMotion(target_azimuth, target_elevation, roll, pitch, yaw):
    # Convert angles from degrees to radians
    yaw = -np.radians(yaw)
    phi = np.radians(roll)
    theta = np.radians(pitch)
    psi = np.radians(yaw)
    azimuth = np.radians(target_azimuth)
    elevation = np.radians(target_elevation)
    # Rotation matrices for roll, pitch, yaw
    Rx = np.array([[1, 0, 0], [0, np.cos(phi), -np.sin(phi)], [0, np.sin(phi), np.cos(phi)]])
    Ry = np.array([[np.cos(theta), 0, np.sin(theta)], [0, 1, 0], [-np.sin(theta), 0, np.cos(theta)]])
    Rz = np.array([[np.cos(psi), np.sin(psi), 0], [-np.sin(psi), np.cos(psi), 0], [0, 0, 1]])
    # Combined rotation matrix
    R = Rz.dot(Ry).dot(Rx)
    #print("R",R)
    # Target vector in local coordinates
    V_local = np.array([np.cos(-elevation) * np.cos(-azimuth), np.cos(-elevation) * np.sin(-azimuth), np.sin(-elevation)])
    #print("V_local:",V_local)
    # Adjust the target vector for the rotation
    V_adjusted = np.linalg.inv(R).dot(V_local)
    #print("V_adjusted",V_adjusted)
    # Calculate new azimuth and elevation from the adjusted vector
    new_azimuth = np.arctan2(V_adjusted[1], V_adjusted[0])
    new_elevation = np.arcsin(V_adjusted[2] / np.linalg.norm(V_adjusted))
    # Convert the new azimuth and elevation back to degrees
    new_azimuth_deg = np.degrees(new_azimuth)
    new_elevation_deg = np.degrees(new_elevation)
    
    return new_azimuth_deg, new_elevation_deg

# Example usage
target_azimuth, target_elevation = 0, 0  # Original target coordinates
roll, pitch, yaw = 10, 20, -100  # Movement measurements including yaw
new_azimuth, new_elevation = adjust_for_pitch_roll_motion(target_azimuth, target_elevation, roll, pitch, yaw)
print("New Azimuth:", new_azimuth, "New Elevation:", new_elevation)
new_azimuth2, new_elevation2 = adjust_for_motion(target_azimuth, target_elevation, roll, pitch, yaw)
print("New Azimuth2:", new_azimuth2, "New Elevation2:", new_elevation2)
new_azimuth3, new_elevation3 = compensateMotion(target_azimuth, target_elevation, roll, pitch, yaw)
print("New Azimuth3:", new_azimuth3, "New Elevation3:", new_elevation3)







