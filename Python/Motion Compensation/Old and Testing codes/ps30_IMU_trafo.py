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
#result1=PS30.PS30_SetStageAttributes(1, nAxis, c_double(1.0), 200, c_double(1.0))

# load param file
if nExport==2:
    result1=PS30.PS30_LoadTextFile(1, ax1, b"ps30_params_export.txt") # ANSI/Unicode !!

# initialize axis
result1=PS30.PS30_MotorInit(1, ax1)
result1=PS30.PS30_MotorInit(1, ax2)

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
time.sleep(0.5)

state1 = PS30.PS30_GetMoveState(1, ax1)
state2 = PS30.PS30_GetMoveState(1, ax2)

while state1 != 0 or state2 != 0:
    state1 = PS30.PS30_GetMoveState(1,ax1)
    state2 = PS30.PS30_GetMoveState(1,ax2)

error = PS30.PS30_SetPosMode(1,ax1,0)
error = PS30.PS30_SetPosMode(1,ax2,0)

# set target mode (0 - relative)
PS30.PS30_SetTargetMode(1, ax1, 0)
PS30.PS30_SetTargetMode(1, ax2, 0)

PS30.PS30_SetTarget(1, ax1, c_double(dZero))
PS30.PS30_SetTarget(1, ax2, c_double(dZero))

result = PS30.PS30_MoveEx(1, ax1, c_double(dZero), 1)
result = PS30.PS30_MoveEx(1, ax2, c_double(dZero), 1)

#save the zero position
z1 = 0.0072 * PS30.PS30_GetPositionEx(1,ax1);
z2 = 0.0072 * PS30.PS30_GetPositionEx(1,ax2);

PS30.PS30_SetPositionEx(1,1,0);
PS30.PS30_SetPositionEx(1,2,0);

print(z1)

    
long_error = PS30.PS30_GetPosVel(1,ax1)  #419430
print("Position velocity axis 1", long_error)
long_error = PS30.PS30_GetPosVel(1,ax2)
print("Position velocity axis 2", long_error)

long_error = PS30.PS30_SetPosVel(1,ax1,8589935)
long_error = PS30.PS30_SetPosVel(1,ax2,8589935)

long_error = PS30.PS30_GetPosVel(1,ax1)  #419430
print("Position velocity axis 1", long_error)
long_error = PS30.PS30_GetPosVel(1,ax2)
print("Position velocity axis 2", long_error)

long_error = PS30.PS30_GetAccel (1,ax1) #2000
print("Position accel axis 1", long_error)
long_error = PS30.PS30_GetAccel (1,ax2,)
print("Position accel axis 2", long_error)

long_error = PS30.PS30_SetAccel(1,ax1,c_double(85610))#95610
long_error = PS30.PS30_SetAccel(1,ax2,c_double(85610))

long_error = PS30.PS30_GetAccel (1,ax1) #2000
print("New Position accel axis 1", long_error)
long_error = PS30.PS30_GetAccel (1,ax2,)
print("New Position accel axis 2", long_error)

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
        return node
        
    except mscl.Error as e:
        print("Error:", e)
        return None

def read_imu_data(node):
    while True:
        try:
            roll, pitch, yaw = np.nan, np.nan, np.nan
            
            # Get all the data packets from the node, with a timeout of 500 milliseconds
            packets= None
            packets = node.getDataPackets(500)
            print("Total packets:",node.totalPackets())
            
            packet = packets[-1]
            # Print out the data
            print("Packet Received: ",packet.data())

            # Iterate over all the data points in the packet
            for dataPoint in packet.data():
                # Print out the channel data
                # Note: The as_string() function is being used here for simplicity.
                # Other methods (i.e., as_float, as_uint16, as_Vector) are also available.
                # To determine the format that a dataPoint is stored in, use dataPoint.storedAs().
                print("Datapoint \n")
                print(dataPoint.channelName() + ":", dataPoint.as_string() + " ", end="")
                print("--\n")

                if not dataPoint.valid():
                    print("[Invalid DataPoint]")
                    continue

                channel = dataPoint.channelName()
                if channel == "roll":
                    roll = math.degrees(dataPoint.as_float())
                elif channel == "pitch":
                    pitch = math.degrees(dataPoint.as_float()) 
                elif channel == "yaw":
                    yaw = math.degrees(dataPoint.as_float()) 

            # print("Roll:", roll," Pitch:",pitch, " Yaw:",yaw, "\n" )
            # If the dataPoint is invalid
            if not dataPoint.valid():
                print("[Invalid] ", end="")

            if not (np.isnan(roll) and np.isnan(pitch) and np.isnan(yaw)):
                return roll, pitch, yaw
            else:
                return None
            
           
        except mscl.Error as e:
            print("Error reading IMU data:", e)
            return None      

def motionOut(target_azimuth, target_elevation, roll, pitch, yaw):
    import numpy as np
    # Convert angles to radians
    phi = np.radians(roll)
    theta = np.radians(pitch)
    psi = np.radians(yaw)
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
    R_inverted = np.linalg.inv(R_combined)   #-----
    corrected_lidar_vector = R_inverted @ lidar_vector

    # Compute real zenith angle
    zenith_angle_rad = np.arccos(corrected_lidar_vector[2] / np.linalg.norm(corrected_lidar_vector))
    zenith_angle_deg = np.degrees(zenith_angle_rad)
    # Compute real azimuth angle
    azimuth_angle_rad = np.arctan2(corrected_lidar_vector[1], corrected_lidar_vector[0])
    azimuth_angle_deg = np.degrees(azimuth_angle_rad) % 360  # Normalize to [0, 360] degrees
    #print("Corrected Lidar Vector:", corrected_lidar_vector)
    #print("Zenith Angle (degrees):", zenith_angle_deg)
    #print("Azimuth Angle (degrees):", azimuth_angle_deg)

    # Compute the azimuth angle in radians from x and y coordinates
    azimuth_rad = np.arctan2(corrected_lidar_vector[1], corrected_lidar_vector[0])
    azimuth_rad = azimuth_rad if azimuth_rad >= 0 else azimuth_rad #+ 2 * np.pi

    # Compute the elevation angle in radians
    # Elevation is the angle from the XY plane, so use hypotenuse in XY as ‘adjacent’ side
    hypotenuse_xy = np.sqrt(corrected_lidar_vector[0]**2 + corrected_lidar_vector[1]**2)
    elevation_rad = np.arctan2(corrected_lidar_vector[2], hypotenuse_xy)
    # Convert radians to degrees
    azimuth_deg = np.degrees(azimuth_rad) % 360 - 180
    elevation_deg = np.degrees(elevation_rad)
    return azimuth_deg, elevation_deg


def main():
    imu_node = configure_imu(imuComPort)
    
    if imu_node is not None:
        while True:
            imu_data = None
            imu_data = read_imu_data(imu_node)

            if imu_data is not None:
                roll = imu_data[0]
                pitch = imu_data[1]
                yaw = imu_data[2]
                
                print("roll:",roll, "pitch:", pitch, "yaw:", yaw)
                
                pts = [(0,0,2)]
                for traj in pts: # this is not correct for intended purpose
                    az = traj[0]
                    elev = traj[1]
                    time = traj[2]
                
                #transform = adjust_for_motion(0, 0, roll, pitch, yaw) # Little to no compensation 
                #transform = motionCompensation(0, 0, roll, pitch, yaw) # No compensation, heavily modified
                #transform = compensateMotion(0, 0, roll, pitch, yaw)
                #transform = compensationRT(0, 0, roll, pitch, yaw)
                transform = motionOut(0, 0, roll, pitch, yaw)
                #transform = moving_target(roll, pitch, yaw, 0, 0)
                print("transform",transform)
                #transform_real = tuple(map(lambda i, j: i - j, transform, (roll, pitch)))
                #print("transform_real", transform_real)


                #transform_final = motionOut_inverse(0, 0, roll, pitch, yaw)
                #transform_final = motionOut_inverse(transform_real[0], transform_real[1], roll, pitch, yaw)
                #transform = motionOut_inverse(transform_real[0], transform_real[1], roll, pitch, yaw)

                ##pos1 = 0.0072 * PS30.PS30_GetPositionEx(1,ax1);   #Review this line
                ##pos2 = 0.0072 * PS30.PS30_GetPositionEx(1,ax2);

                ##print("here z1 is:", pos1)
                ##print("here z2 is:", pos2)
                
                #dDistance_az =  pos1 - roll  #transform[0]
                #dDistance_elev = pos2 - pitch #transform[1]

                dDistance_az =   transform[0] # roll  #
                dDistance_elev = transform[1]   # pitch #
                #dDistance_az1 =   transform[0] # roll  #
                #dDistance_elev1 = transform[1]   # pitch #
                #print(dDistance_az1)
                #print(dDistance_elev1)

                if abs(dDistance_az) > 24:
                    dDistance_az = 24 * (-1 if dDistance_az < 0 else 1)
                if abs(dDistance_elev) > 24:
                    dDistance_elev = 24 * (-1 if dDistance_elev < 0 else 1)

                ##print("dDistance_az:", dDistance_az)
                ##print("dDistance_elev:",dDistance_elev)
                
                PS30.PS30_SetTargetMode(1, ax1, 1)
                PS30.PS30_SetTargetMode(1, ax2, 1)

                PS30.PS30_SetTargetEx(1, ax1, c_double(dDistance_az))
                PS30.PS30_SetTargetEx(1, ax2, c_double(dDistance_elev))

                #result = PS30.PS30_MoveEx(1, ax1, c_double(dDistance_az), 1)
                #result = PS30.PS30_MoveEx(1, ax2, c_double(dDistance_elev), 1)

                #result = PS30.PS30_GoTarget(1, ax1)
                #result = PS30.PS30_GoTarget(1, ax2)

                result = PS30.PS30_MultiGoTarget(1, 3,1)
                

    else:
        print("IMU configuration failed")


if __name__ == "__main__":
    main()
    