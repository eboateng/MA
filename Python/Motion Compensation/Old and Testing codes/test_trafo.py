# Import the mscl library
import sys
import time
sys.path.append("../../dependencies/Python")
import mscl
import math
import numpy as np
#from filterpy.kalman import KalmanFilter

# TODO: change these constants to match your setup
COM_PORT = "COM5"
roll, pitch, yaw = np.nan, np.nan, np.nan
print("Let's begin")

def adjust_for_motion(target_azimuth, target_elevation, roll, pitch, yaw):
    # Convert angles from degrees to radians
    yaw = 0
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
    print("R",R)
    # Target vector in local coordinates
    V_local = np.array([np.cos(-elevation) * np.cos(-azimuth), np.cos(-elevation) * np.sin(-azimuth), np.sin(-elevation)])
    print("V_local:",V_local)
    # Adjust the target vector for the rotation
    V_adjusted = np.linalg.inv(R).dot(V_local)
    print("V_adjusted",V_adjusted)
    # Calculate new azimuth and elevation from the adjusted vector
    new_azimuth = np.arctan2(V_adjusted[1], V_adjusted[0])
    new_elevation = np.arcsin(V_adjusted[2] / np.linalg.norm(V_adjusted))
    # Convert the new azimuth and elevation back to degrees
    new_azimuth_deg = np.degrees(new_azimuth)
    new_elevation_deg = np.degrees(new_elevation)
    
    return new_azimuth_deg, new_elevation_deg

def motionOut(target_azimuth, target_elevation, roll, pitch, yaw):
    # Convert angles to radians for internal calculations
    phi = np.radians(roll)    # Roll
    theta = np.radians(pitch) # Pitch
    psi = np.radians(yaw)     # Yaw
    yaw_offset = 180
    # Correct the yaw angle by subtracting the offset
    psi_corrected = psi - np.radians(yaw_offset)
    psi = psi_corrected
    print("corrected yaw", np.degrees(psi))

    # Convert target azimuth and elevation to Cartesian coordinates for the lidar vector
    lidar_vector = np.array([
        np.cos(np.radians(target_elevation)) * np.cos(np.radians(target_azimuth)),
        np.cos(np.radians(target_elevation)) * np.sin(np.radians(target_azimuth)),
        np.sin(np.radians(target_elevation))
    ])

    print("Lidar vector:", lidar_vector)

    # Rotation matrices
    R_z = np.array([
        [np.cos(psi), -np.sin(psi), 0],
        [np.sin(psi), np.cos(psi), 0],
        [0, 0, 1]])
    
    R_y = np.array([
        [np.cos(theta), 0, np.sin(theta)],
        [0, 1, 0],
        [-np.sin(theta), 0, np.cos(theta)]])
    
    R_x = np.array([
        [1, 0, 0],
        [0, np.cos(phi), -np.sin(phi)],
        [0, np.sin(phi), np.cos(phi)]])
    
    #print("R_z (yaw):", R_z)
    #print("R_y (pitch):", R_y)
    #print("R_x (roll):", R_x)

    # Combined rotation matrix
    R_combined = R_z @ R_y @ R_x
    print("R_combined:", R_combined)

    # Adjust lidar vector for motion
    corrected_lidar_vector = R_combined @ lidar_vector
    print("Corrected Lidar Vector:", corrected_lidar_vector)

    # Convert corrected_lidar_vector back to spherical coordinates
    azimuth_angle_rad = np.arctan2(corrected_lidar_vector[1], corrected_lidar_vector[0])
    azimuth_angle_deg = np.degrees(azimuth_angle_rad) % 360

    # Normalize azimuth to [0, 360] degrees
    #if azimuth_angle_deg < 0:
    #    azimuth_angle_deg += 360
    # Elevation is the angle from the horizontal plane
    hypotenuse_xy = np.sqrt(corrected_lidar_vector[0]**2 + corrected_lidar_vector[1]**2)
    elevation_angle_rad = np.arctan2(corrected_lidar_vector[2], hypotenuse_xy)
    elevation_angle_deg = np.degrees(elevation_angle_rad)
    # Print intermediate results for debugging
    print(f"Roll: {roll}, Pitch: {pitch}, Yaw: {yaw}")
    print("corrected_lidar_vector", corrected_lidar_vector)
    print("azimuth_angle_deg & elevation_angle_deg", (azimuth_angle_deg, elevation_angle_deg))
    return azimuth_angle_deg, elevation_angle_deg

# def motionOut(target_azimuth, target_elevation, roll, pitch, yaw): # 30.05
#     import numpy as np
#     # Convert angles to radians
#     phi = np.radians(roll)
#     theta = np.radians(pitch)
#     psi = np.radians(yaw)
#     yaw_offset = 174
#     # Correct the yaw angle by subtracting the offset
#     psi_corrected = psi - np.radians(yaw_offset)
#     psi = 0 #psi_corrected

#     # Convert target azimuth and elevation to Cartesian coordinates for the lidar vector
#     lidar_vector = np.array([
#         np.cos(np.radians(target_elevation)) * np.cos(np.radians(target_azimuth)),
#         np.cos(np.radians(target_elevation)) * np.sin(np.radians(target_azimuth)),
#         np.sin(np.radians(target_elevation))
#     ])

    
#     # Rotation matrices
#     R_z = np.array([
#         [np.cos(psi), -np.sin(psi), 0],
#         [np.sin(psi), np.cos(psi), 0],
#         [0, 0, 1]])
#     R_y = np.array([
#         [np.cos(theta), 0, np.sin(theta)],
#         [0, 1, 0],
#         [-np.sin(theta), 0, np.cos(theta)]])
#     R_x = np.array([
#         [1, 0, 0],
#         [0, np.cos(phi), -np.sin(phi)],
#         [0, np.sin(phi), np.cos(phi)]])
    

#     # Combined rotation matrix
#     R_combined = R_z @ R_y @ R_x

#     # Adjust lidar vector for motion
#     corrected_lidar_vector = np.linalg.inv(R_combined) @ lidar_vector

#     # Compute real azimuth and elevation angles from corrected vector
#     azimuth_angle_rad = np.arctan2(corrected_lidar_vector[1], corrected_lidar_vector[0])
#     elevation_angle_rad = np.arctan2(corrected_lidar_vector[2],np.sqrt(corrected_lidar_vector[0]**2 + corrected_lidar_vector[1]**2))

#     # Convert radians to degrees
#     azimuth_angle_deg =np.degrees(azimuth_angle_rad)
#     elevation_angle_deg = np.degrees(elevation_angle_rad)

#     return azimuth_angle_deg, elevation_angle_deg

# def motionOut(target_azimuth, target_elevation, roll, pitch, yaw):
#     import numpy as np
#     # Convert angles to radians
#     phi = np.radians(roll)
#     theta = np.radians(pitch)
#     psi = np.radians(yaw)
#     # Convert target azimuth and elevation to Cartesian coordinates for the lidar vector
#     lidar_vector = np.array([
#         np.cos(np.radians(target_elevation)) * np.cos(np.radians(target_azimuth)),
#         np.cos(np.radians(target_elevation)) * np.sin(np.radians(target_azimuth)),
#         np.sin(np.radians(target_elevation))
#     ])
#     # Rotation matrices
#     R_z = np.array([
#         [np.cos(psi), -np.sin(psi), 0],
#         [np.sin(psi), np.cos(psi), 0],
#         [0, 0, 1]])
#     R_y = np.array([
#         [np.cos(theta), 0, np.sin(theta)],
#         [0, 1, 0],
#         [-np.sin(theta), 0, np.cos(theta)]])
#     R_x = np.array([
#         [1, 0, 0],
#         [0, np.cos(phi), -np.sin(phi)],
#         [0, np.sin(phi), np.cos(phi)]])
#     # Combined rotation matrix
#     R_combined = R_z @ R_y @ R_x # including yaw
#     #R_combined = R_y @ R_x  # ignoring yaw

#     # Adjust lidar vector for motion
#     corrected_lidar_vector = R_combined @ lidar_vector
#     # Compute real zenith angle
#     zenith_angle_rad = np.arccos(corrected_lidar_vector[2] / np.linalg.norm(corrected_lidar_vector))
#     zenith_angle_deg = np.degrees(zenith_angle_rad)
#     # Compute real azimuth angle
#     #azimuth_angle_rad = np.arctan2(corrected_lidar_vector[1], corrected_lidar_vector[0])
#     #azimuth_angle_deg = np.degrees(azimuth_angle_rad) % 360  # Normalize to [0, 360] degrees
#     print("Corrected Lidar Vector:", corrected_lidar_vector)
#     #print("Zenith Angle (degrees):", zenith_angle_deg)
#     #print("Azimuth Angle (degrees):", azimuth_angle_deg)

#     # Compute the azimuth angle in radians from x and y coordinates
#     azimuth_rad = np.arctan2(corrected_lidar_vector[1], corrected_lidar_vector[0])
#     azimuth_rad = azimuth_rad if azimuth_rad >= 0 else azimuth_rad #+ 2 * np.pi

#     # Compute the elevation angle in radians
#     # Elevation is the angle from the XY plane, so use hypotenuse in XY as ‘adjacent’ side
#     hypotenuse_xy = np.sqrt(corrected_lidar_vector[0]**2 + corrected_lidar_vector[1]**2)
#     elevation_rad = np.arctan2(corrected_lidar_vector[2], hypotenuse_xy)
#     # Convert radians to degrees
#     azimuth_deg = np.degrees(azimuth_rad) %360 - 180
#     elevation_deg = np.degrees(elevation_rad)
#     return azimuth_deg, elevation_deg

try:
    # Create a Serial Connection with the specified COM Port, default baud rate of 921600
    connection = mscl.Connection.Serial(COM_PORT)
    
    # Create an InertialNode with the connection
    node = mscl.InertialNode(connection)

    # Many other settings are available than shown below
    # Reference the documentation for the full list of commands

    # If the node supports AHRS/IMU
    if node.features().supportsCategory(mscl.MipTypes.CLASS_AHRS_IMU):
        ahrsImuChs = mscl.MipChannels()
        ahrsImuChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_SENSOR_EULER_ANGLES, mscl.SampleRate.Hertz(50)))
        ahrsImuChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_SENSOR_SCALED_ACCEL_VEC, mscl.SampleRate.Hertz(50)))
        ahrsImuChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_SENSOR_SCALED_GYRO_VEC, mscl.SampleRate.Hertz(50)))
        ahrsImuChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_SENSOR_SCALED_MAG_VEC, mscl.SampleRate.Hertz(50)))

        # Apply to the node
        node.setActiveChannelFields(mscl.MipTypes.CLASS_AHRS_IMU, ahrsImuChs)

    # If the node supports Estimation Filter
    if node.features().supportsCategory(mscl.MipTypes.CLASS_ESTFILTER):
        estFilterChs = mscl.MipChannels()
        estFilterChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_ESTFILTER_ESTIMATED_ORIENT_EULER, mscl.SampleRate.Hertz(1)))

        # Apply to the node
        node.setActiveChannelFields(mscl.MipTypes.CLASS_ESTFILTER, estFilterChs)

    # If the node supports GNSS
    if node.features().supportsCategory(mscl.MipTypes.CLASS_GNSS):
        gnssChs = mscl.MipChannels()
        gnssChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_GNSS_LLH_POSITION, mscl.SampleRate.Hertz(1)))

        # Apply to the node
        node.setActiveChannelFields(mscl.MipTypes.CLASS_GNSS, gnssChs)

    node.setPitchRollAid(True)

    node.setAltitudeAid(True)

    offset = mscl.PositionOffset(0.0, 0.0, 0.0)
    node.setAntennaOffset(offset)
    print("Rolling")
    

    while True:
        packets = node.getDataPackets(500)
        if not packets:
            print("No packets received.")
            time.sleep(5)
            continue

        roll, pitch, yaw = np.nan, np.nan, np.nan

        for packet in packets:
            #print("Packet Received:")
            for dataPoint in packet.data():
                #print(f"{dataPoint.channelName()}: {dataPoint.as_string()} ", end="")

                if not dataPoint.valid():
                    #print("[Invalid DataPoint]")
                    continue

                channel = dataPoint.channelName()
                if channel == "roll":
                    roll = math.degrees(dataPoint.as_float())
                elif channel == "pitch":
                    pitch = math.degrees(dataPoint.as_float())
                elif channel == "yaw":
                    yaw = math.degrees(dataPoint.as_float())

        if not (np.isnan(roll) and np.isnan(pitch) and np.isnan(yaw)):
            print(f"\nRoll: {roll}, Pitch: {pitch}, Yaw: {yaw}")
            cTransform = motionOut(0.01, 0.01, roll, pitch, yaw)
            print("New Az & Ele", cTransform)
        else:
            print("\nNot all orientation values were updated.")

        time.sleep(1)

except mscl.Error as e:
    print("Error:", e)


