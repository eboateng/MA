import csv
import ctypes
from ctypes import windll, c_double
import sys
import time
import platform
sys.path.append("../../dependencies/Python") 
#sys.path.append("C:\\Python38/DLLs/")
import mscl
import math
from datetime import datetime
import keyboard  

imuComPort = "COM5"


def configure_imu(imuComPort):

    try:
        # Create a Serial Connection with the specified COM Port, default baud rate of 921600
        connection = mscl.Connection.Serial(imuComPort)
        
        # Create an InertialNode with the connection
        node = mscl.InertialNode(connection)

        # Configure the node as per your requirements
        
        return node
        
    except mscl.Error as e:
        print("Error:", e)
        return None

def read_imu_data(node):
    try:
        roll, pitch, yaw = 0, 0, 0
        ac_x, ac_y, ac_z = 0, 0, 0
        gy_x, gy_y, gy_z = 0, 0, 0
        
        # Get all the data packets from the node, with a timeout of 500 milliseconds
        packets= None
        packets = node.getDataPackets(500)
            
        #print("Total packets:",node.totalPackets())
        
        packet = packets[-1]
        
        #for packet in packets:
            # Iterate over all the data points in the packet
        for dataPoint in packet.data():
            channel = dataPoint.channelName()
            if channel == "roll":
                roll = math.degrees(dataPoint.as_float())
            elif channel == "pitch":
                pitch = math.degrees(dataPoint.as_float()) 
            elif channel == "yaw":
                yaw = math.degrees(dataPoint.as_float()) 
            elif channel == "scaledAccelX":
                ac_x = dataPoint.as_float()
            elif channel == "scaledAccelY":
                ac_y = dataPoint.as_float()
            elif channel == "scaledAccelZ":
                ac_z = dataPoint.as_float()
            elif channel == "scaledGyroX":
                gy_x = dataPoint.as_float()
            elif channel == "scaledGyroY":
                gy_y = dataPoint.as_float()
            elif channel == "scaledGyroZ":
                gy_z = dataPoint.as_float()

        if roll == 0 or pitch == 0 or yaw == 0:
            return None
        else:
            return (roll, pitch, yaw, ac_x, ac_y, ac_z, gy_x, gy_y, gy_z)
           
    except mscl.Error as e:
        print("Error reading IMU data:", e)
        return None  

# Create and open a CSV file in write mode
csv_filename = "DHsensor_data.csv"
with open(csv_filename, mode='w', newline='') as csv_file:
    fieldnames = ['sTimestamp', 'sRoll', 'sPitch', 'sYaw', 'sAcc_x','sAcc_y','sAcc_z', 'sGyro_x', 'sGyro_y', 'sGyro_z']
    writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
    writer.writeheader()

    def main():
        imu_node = configure_imu(imuComPort)
        if imu_node is not None:
            while True:
                if keyboard.is_pressed('esc'):
                    print("Escape key pressed. Stopping data logging.")
                    break  # Break out of the loop if escape key is pressed

                imu_data = read_imu_data(imu_node)

                if imu_data is not None:
                    roll = imu_data[0]
                    pitch = imu_data[1]
                    yaw = imu_data[2]
                    Acc_x = imu_data[3]
                    Acc_y = imu_data[4]
                    Acc_z = imu_data[5]
                    Gyro_x  = imu_data[6]
                    Gyro_y  = imu_data[7]
                    Gyro_z  = imu_data[8]

                    print(imu_data)

                    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
                    writer.writerow({'sTimestamp': timestamp, 'sRoll': roll, 'sPitch': pitch, 'sYaw': yaw, 'sAcc_x': Acc_x,'sAcc_y': Acc_y,'sAcc_z': Acc_z, 'sGyro_x': Gyro_x, 'sGyro_y': Gyro_y, 'sGyro_z': Gyro_z})
                    

        else:
            print("IMU configuration failed")

    if __name__ == "__main__":
        main()

# Close the CSV file
csv_file.close()
