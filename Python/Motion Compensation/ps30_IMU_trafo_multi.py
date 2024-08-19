# -*- coding: utf-8 -*-
"""
Created on Wed Aug 17 11:06:45 2016

@author: as
"""

import ctypes
#from ctypes import *
from ctypes import windll, c_double
#from ctypes import windll, c_double, create_string_buffer
import sys
import time
sys.path.append("../../dependencies/Python")
import mscl
import math
import numpy as np
import multiprocessing
import threading
import queue

# PS30 Globals
ps30ComPort=2 
ax1=1 #axes 1
ax2=2 # axes 2
dPosF=3000
dDistance_az=0
dDistance_elev=0
dZero = 26 
nExport=0
nAxis = 2
imu_data = []
data_lock = threading.Lock()
data_available = threading.Condition(data_lock)

running = True # Terminate process flag


# IMU Globals
imuComPort = "COM5"
#roll, pitch, yaw = 0, 0, 0


# load library
# give location of dll
PS30 = windll.LoadLibrary("ps30.dll")

# open virtual serial interface (or serial interface via tcp/ip socket)
if ps30ComPort==0: # find first connected control unit
    result1=PS30.PS30_SimpleConnect(1, b"") # ANSI/Unicode !!
elif ps30ComPort==-1: # find the first connected control unit via tcp/ip socket (localhost, port=1200)
    result1=PS30.PS30_SimpleConnect(1, b"net") # ANSI/Unicode !!
else: # connect control unit with defined COM port
    result1=PS30.PS30_Connect(1, 0, ps30ComPort, 115200,0,0,0,0)

# define constants for calculation Inc -> mm
#result1=PS30.PS30_SetStageAttributes(1, ax1, c_double(360.0), 200, c_double(180))
#result1=PS30.PS30_SetStageAttributes(1, ax2, c_double(360.0), 200, c_double(180))

#result1=PS30.PS30_SetCalcResol(1,ax1,c_double(0.0001))
#result1=PS30.PS30_SetCalcResol(1,ax2,c_double(0.0001))


# load param file
if nExport==2:
    result1=PS30.PS30_LoadTextFile(1, ax1, b"ps30_params_export.txt") # ANSI/Unicode !!

# initialize axis
result1=PS30.PS30_MotorInit(1, ax1)
result1=PS30.PS30_MotorInit(1, ax2)
print("Motors initialized")

# save param file
if nExport==1:
    result1=PS30.PS30_SaveTextFile(1, ax1, b"ps30_params_export.txt") # ANSI/Unicode !!



##PS30_GetPositionEx=PS30.PS30_GetPositionEx
##PS30_GetPositionEx.restype = ctypes.c_double
##result2=PS30_GetPositionEx(1, nAxis)
##result3=PS30_GetPositionEx(1, 1)
##print("Position=%.3f" %(result2))

#check if reference run was previously executed
# TBD

##start positioning
PS30.PS30_GoRef(1, ax1, 6)
PS30.PS30_GoRef(1, ax2, 6)
print("Starting reference run")
time.sleep(0.5)

state1 = PS30.PS30_GetMoveState(1, ax1)
state2 = PS30.PS30_GetMoveState(1, ax2)

print("running...")
while state1 != 0 or state2 != 0:
    state1 = PS30.PS30_GetMoveState(1,ax1)
    state2 = PS30.PS30_GetMoveState(1,ax2)
print("Reference run complete")
error = PS30.PS30_SetPosMode(1,ax1,0)
error = PS30.PS30_SetPosMode(1,ax2,0)

# set target mode (0 - relative)
PS30.PS30_SetTargetMode(1, ax1, 0)
PS30.PS30_SetTargetMode(1, ax2, 0)

PS30.PS30_SetTarget(1, ax1, c_double(dZero))
PS30.PS30_SetTarget(1, ax2, c_double(dZero))
print("System ready")

result = PS30.PS30_MoveEx(1, ax1, c_double(dZero), 1)
result = PS30.PS30_MoveEx(1, ax2, c_double(dZero), 1)

#save the zero position
z1 = 0.0072 * PS30.PS30_GetPositionEx(1,ax1)
#z1 = PS30.PS30_GetPositionEx(1,ax1)
z2 = 0.0072 * PS30.PS30_GetPositionEx(1,ax2)
print(z1)

PS30.PS30_SetPositionEx(1,ax1,0);
PS30.PS30_SetPositionEx(1,ax2,0);

long_error = PS30.PS30_GetPosVel(1,ax1)  #419430
print("Position velocity axis 1", long_error)
long_error = PS30.PS30_GetPosVel(1,ax2)
print("Position velocity axis 2", long_error)

long_error = PS30.PS30_SetPosVel(1,ax1, 419430) # increased velocity(see docu) = 8589935
long_error = PS30.PS30_SetPosVel(1,ax2,419430)

long_error = PS30.PS30_GetPosVel(1,ax1)  # 419430
print("Position velocity axis 1", long_error)
long_error = PS30.PS30_GetPosVel(1,ax2)
print("Position velocity axis 2", long_error)

long_error = PS30.PS30_GetAccel (1,ax1) #2000
print("Position accel axis 1", long_error)
long_error = PS30.PS30_GetAccel (1,ax2,)
print("Position accel axis 2", long_error)

long_error = PS30.PS30_SetAccel(1,ax1,c_double(2000)) ##increased acc(see docu) = 95610 my setting
long_error = PS30.PS30_SetAccel(1,ax2,c_double(2000))

long_error = PS30.PS30_GetAccel (1,ax1) #2000
print("New Position accel axis 1", long_error)
long_error = PS30.PS30_GetAccel (1,ax2,)
print("New Position accel axis 2", long_error)

long_error = PS30.PS30_SetPowerOutput(1,ax1,2)
long_error = PS30.PS30_SetPowerOutput(1,ax2,2)

#configure INS sensor
def configure_imu(imuComPort):

    try:
        # Create a Serial Connection with the specified COM Port, default baud rate of 921600
        connection = mscl.Connection.Serial(imuComPort)
        
        # Create an InertialNode with the connection
        node = mscl.InertialNode(connection)

        # Many other settings are available than shown below
        # Reference the documentation for the full list of commands

        # If the node supports AHRS/IMU
        if node.features().supportsCategory(mscl.MipTypes.CLASS_AHRS_IMU):
            ahrsImuChs = mscl.MipChannels()
            ahrsImuChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_SENSOR_EULER_ANGLES, mscl.SampleRate.Hertz(25))) #50
            ahrsImuChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_SENSOR_SCALED_ACCEL_VEC, mscl.SampleRate.Hertz(25)))
            ahrsImuChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_SENSOR_SCALED_GYRO_VEC, mscl.SampleRate.Hertz(25)))
            ahrsImuChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_SENSOR_SCALED_MAG_VEC, mscl.SampleRate.Hertz(25)))
            ahrsImuChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_SENSOR_DELTA_THETA_VEC, mscl.SampleRate.Hertz(25)))

            # Apply to the node
            node.setActiveChannelFields(mscl.MipTypes.CLASS_AHRS_IMU, ahrsImuChs)

        # If the node supports Estimation Filter
        if node.features().supportsCategory(mscl.MipTypes.CLASS_ESTFILTER):
            estFilterChs = mscl.MipChannels()
            estFilterChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_ESTFILTER_ESTIMATED_ORIENT_EULER, mscl.SampleRate.Hertz(25))) #1

            # Apply to the node
            node.setActiveChannelFields(mscl.MipTypes.CLASS_ESTFILTER, estFilterChs)

        # If the node supports GNSS
        if node.features().supportsCategory(mscl.MipTypes.CLASS_GNSS):
            gnssChs = mscl.MipChannels()
            gnssChs.append(mscl.MipChannel(mscl.MipTypes.CH_FIELD_GNSS_LLH_POSITION, mscl.SampleRate.Hertz(1))) #1

            # Apply to the node
            node.setActiveChannelFields(mscl.MipTypes.CLASS_GNSS, gnssChs)

        node.setPitchRollAid(True)

        node.setAltitudeAid(True)

        offset = mscl.PositionOffset(0.0, 0.0, 0.0)
        node.setAntennaOffset(offset)
        return node
        
    except mscl.Error as e:
        print("Error:", e)
        return None

#read INS sensor data
def read_imu_data(imu_node, data_queue):
    global running, imu_data # shared termination flag and data buffer

    node = imu_node
    # print("node is", node)

    try:
        while running:
            roll, pitch, yaw = np.nan, np.nan, np.nan
            dx, dy, dz = np.nan, np.nan, np.nan
            
            # Get all the data packets from the node, with a timeout of 500 milliseconds
            packets= None
            packets = node.getDataPackets(500)
            
            #print("Total packets:",node.totalPackets())
            
            packet = packets[-1]
            # Print out the data
            #print("Packet Received: ",packet.data())

            # Iterate over all the data points in the packet
            for dataPoint in packet.data():
                # Print out the channel data
                # Note: The as_string() function is being used here for simplicity.
                # Other methods (i.e., as_float, as_uint16, as_Vector) are also available.
                # To determine the format that a dataPoint is stored in, use dataPoint.storedAs().
                #print("Datapoint \n")
                #print(dataPoint.channelName() + ":", dataPoint.as_string() + " ", end="")
                #print("--\n")

                # if not dataPoint.valid():
                #     print("[Invalid DataPoint]")
                #     continue

                channel = dataPoint.channelName()
                #print(channel)
                if channel == "roll":
                    roll = math.degrees(dataPoint.as_float())
                elif channel == "pitch":
                    pitch = math.degrees(dataPoint.as_float()) 
                elif channel == "yaw":
                    yaw = 1*math.degrees(dataPoint.as_float())
                elif channel == "deltaThetaX":
                    dx = math.degrees(dataPoint.as_float())
                elif channel == "deltaThetaY":
                    dy = math.degrees(dataPoint.as_float())
                elif channel == "deltaThetaZ":
                    dz = math.degrees(dataPoint.as_float())
                
            #print("Roll:", roll," Pitch:",pitch, " Yaw:",yaw, "\n" )
            #print("dx:", dx," dy:",dy, " dz:",dz, "\n" )

            # If the dataPoint is invalid
            if not dataPoint.valid():
                print("[Invalid] ", end="")

            if not (np.isnan(roll) and np.isnan(pitch) and np.isnan(yaw) and np.isnan(dx) and np.isnan(dy) and np.isnan(dz)):
                with data_available:
                    imu_data = [roll, pitch, yaw, dx, dy, dz]
                    data_available.notify()
                    #print("Roll:", roll," Pitch:",pitch, " Yaw:",yaw, "\n" )
                #return roll, pitch, yaw
            # else:
            #     return None
            time.sleep(0.01) 

    except mscl.Error as e:
        print("Stopped due to:", e)
        running = False
        return None      

# coordinate transform 
def motionOut(target_azimuth, target_elevation, roll, pitch, yaw):
    import numpy as np
    # Convert angles to radians
    phi = np.radians(roll)
    theta = np.radians(pitch)
    psi = np.radians(yaw)
    yaw_offset = 180
    # Correct the yaw angle by subtracting the offset
    psi_corrected = psi - np.radians(yaw_offset)
    psi = psi_corrected

    # Convert target azimuth and elevation to Cartesian coordinates for the lidar vector
    lidar_vector = np.array([
        np.cos(np.radians(target_elevation)) * np.cos(np.radians(target_azimuth)),
        np.cos(np.radians(target_elevation)) * np.sin(np.radians(target_azimuth)),
        np.sin(np.radians(target_elevation))
    ])

    
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
    

    # Combined rotation matrix
    R_combined = R_z @ R_y @ R_x

    # Adjust lidar vector for motion
    corrected_lidar_vector = np.linalg.inv(R_combined) @ lidar_vector

    # Compute real azimuth and elevation angles from corrected vector
    azimuth_angle_rad = np.arctan2(corrected_lidar_vector[1], corrected_lidar_vector[0])
    elevation_angle_rad = np.arcsin(corrected_lidar_vector[2] / np.linalg.norm(corrected_lidar_vector))

    # Convert radians to degrees
    azimuth_angle_deg = np.degrees(azimuth_angle_rad)
    elevation_angle_deg = np.degrees(elevation_angle_rad)

    # Calculate azimuth and elevation shifts based on roll and pitch
    #azimuth_shift = roll #* np.cos(target_elevation)
    #elevation_shift = pitch #* np.cos(target_azimuth)

    # Apply the azimuth and elevation shifts
    #azimuth_angle_deg -= azimuth_shift
    #elevation_angle_deg -= elevation_shift 

    return azimuth_angle_deg, elevation_angle_deg

#Danevit-Hartenberg transformation
def dh_transformation_matrix(a, alpha, d, theta):
  """"
  Generate the Denavit-Hartenberg transformation matrix.
  Parameters:
  a (float): Link length
  alpha (float): Link twist
  d (float): Link offset
  theta (float): Joint angle
  Returns:
  np.array: The 4x4 transformation matrix
  """

  return np.array([
    [np.cos(theta), -np.sin(theta) * np.cos(alpha), np.sin(theta) * np.sin(alpha), a * np.cos(theta)],
    [np.sin(theta), np.cos(theta) * np.cos(alpha), -np.cos(theta) * np.sin(alpha), a * np.sin(theta)],
    [0, np.sin(alpha), np.cos(alpha), d],
    [0, 0, 0, 1]
  ])
def extract_rpy_from_transformation(T):
  """"
  Extract roll, pitch, and yaw from a transformation matrix.
  Parameters:
  T (np.array): The 4x4 transformation matrix
  Returns:
  tuple: Roll, pitch, and yaw angles in degrees
  """

  sy = np.sqrt(T[0, 0] ** 2 + T[1, 0] ** 2)
  singular = sy < 1e-6
  if not singular:
    roll = np.arctan2(T[2, 1], T[2, 2])
    pitch = np.arctan2(-T[2, 0], sy)
    yaw = np.arctan2(T[1, 0], T[0, 0])
  else:
    roll = np.arctan2(-T[1, 2], T[1, 1])
    pitch = np.arctan2(-T[2, 0], sy)
    yaw = 0
  return np.degrees(roll), np.degrees(pitch), np.degrees(yaw)
def imu_to_dh_transformation(roll, pitch, yaw):
    """
    Create the transformation matrix from IMU roll, pitch, and yaw data.
    Parameters:
    roll (float): Roll angle in degrees
    pitch (float): Pitch angle in degrees
    yaw (float): Yaw angle in degrees
    Returns:
    np.array: The final 4x4 transformation matrix
    """
    # Convert angles to radians
    roll = np.radians(roll)
    pitch = np.radians(pitch)
    yaw = np.radians(yaw)
    # Assuming a_i = 0 and d_i = 0 for simplicity
    a, alpha, d = 0, 0, 0
    # Create the transformation matrices for roll, pitch, and yaw
    T_roll = dh_transformation_matrix(a, alpha, d, roll)
    T_pitch = dh_transformation_matrix(a, alpha, d, pitch)
    T_yaw = dh_transformation_matrix(a, alpha, d, yaw)
    # Combine the transformations (typically ZYX order: yaw, then pitch, then roll)
    T_final = T_yaw @ T_pitch @ T_roll
    return T_final

def main(data_queue):
    current_roll, current_pitch, current_yaw = 0, 0, 0
    last_ts = time.time()
    
    global running        
    try:
        while running:
            # Retrieve data from the queue
            with data_available:
                data_available.wait()  # Wait until data is available
                roll, pitch, yaw, delta_roll, delta_pitch, delta_yaw = imu_data

                #print(f"Roll: {roll}, Pitch: {pitch}, Yaw: {yaw}")

                # Calculate delta_time
                #current_time = time.time()
                #delta_time = current_time - last_ts
                #print("delta_time",delta_time)
                #last_ts = current_time

				# predict position based on delta RPY (add directly or based on delta time)
                #c_roll += delta_roll * 0.4
                #c_pitch += delta_pitch * 0.4
                #c_yaw += delta_yaw * 0.4

                #roll += delta_roll
                #pitch += delta_pitch
                #yaw += delta_yaw


                print("dRoll:", roll," dPitch:",pitch, " dYaw:",yaw, "\n" )
                azimuth = 0
                elevation = 0
                T_final = imu_to_dh_transformation(roll, pitch, yaw)
                tr_roll, tr_pitch, tr_yaw = extract_rpy_from_transformation(T_final)
                transform = motionOut(azimuth, elevation, roll, pitch, yaw)

                #Get current position for relative move command
                ax1_pos = 0.0072 * PS30.PS30_GetPositionEx(1,ax1)
                ax2_pos = 0.0072 * PS30.PS30_GetPositionEx(1,ax2)

                print("ax1_pos", ax1_pos)
                #print("ax2_pos", ax2_pos)

                dDistance_az = transform[0]#  + azimuth #+ ax1_pos # roll  #
                dDistance_elev = -transform[1] #- pitch + elevation #+ ax2_pos # pitch #

                print("Transformed position:", transform)
                print(dDistance_az)
                #print(dDistance_elev)

                if abs(dDistance_az) > 24:
                    dDistance_az = 24 * (-1 if dDistance_az < 0 else 1)
                if abs(dDistance_elev) > 24:
                    dDistance_elev = 24 * (-1 if dDistance_elev < 0 else 1)

                # ##print("dDistance_az:", dDistance_az)
                # ##print("dDistance_elev:",dDistance_elev)
                
                PS30.PS30_SetTargetMode(1, ax1, 1)
                PS30.PS30_SetTargetMode(1, ax2, 1)

                PS30.PS30_SetTargetEx(1, ax1, c_double(dDistance_az))
                PS30.PS30_SetTargetEx(1, ax2, c_double(dDistance_elev))

                #result = PS30.PS30_MoveEx(1, ax1, c_double(dDistance_az), 1)
                #result = PS30.PS30_MoveEx(1, ax2, c_double(dDistance_elev), 1)

                #result = PS30.PS30_GoTarget(1, ax1)
                #result = PS30.PS30_GoTarget(1, ax2)

                result = PS30.PS30_MultiGoTarget(1, 3,1)
                #time.sleep(0.005)

    except KeyboardInterrupt:
        print("Main process interrupted.")
        running = False


if __name__ == "__main__":
    imu_node = configure_imu(imuComPort) 
    
    if imu_node is not None:
        # Create a queue for sharing data between threads
        imu_data_queue = queue.Queue()

        # Start the sensor data reading thread
        sensor_thread = threading.Thread(target=read_imu_data, args=(imu_node, imu_data_queue))
        sensor_thread.start()

        try:
            # Run the main processing loop
            main(imu_data_queue)

        finally:
             # Wait for the sensor thread to finish (only happens on interrupt)
            sensor_thread.join()
            running = False
    else:
        print("Failed to configure IMU.")