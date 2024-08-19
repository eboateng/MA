"""
This script controls a motion platform using the ForceSeatMI SDK, allowing for various motion modes such as surge, sway, heave, roll, pitch, and yaw. It can use existing data from a .mat file or generate new data for the motion. The script records the platform's movement data in a CSV file and allows for real-time keyboard interruption to stop the motion.

Motion modes can be set to control specific combinations of movements. The script supports threading to listen for a keyboard interrupt, enabling the user to stop the motion by pressing the 'Esc' key.

Developed by Emmanuel Boateng, 2024. All rights reserved.
"""

import time
import math
import csv
from ForceSeatMI import *
from datetime import datetime
import threading
import keyboard
import numpy as np
import scipy.io

# Initialize the keyboard interrupt flag
motion_interrupt = False

# Flag to determine if existing data should be used
use_existing_data = False

# Variable to store the selected motion mode
motion_mode = 0

# Motion modes mapping
'''
    Motion Modes:
    -------------
    Mode 1 : surge                  Mode 4 : roll
    Mode 2 : sway                   Mode 5 : pitch
    Mode 3 : heave                  Mode 6 : yaw

    Mode 7 : surge + roll           Mode 10 : sway + roll
    Mode 8 : surge + pitch          Mode 11 : sway + pitch
    Mode 9 : surge + yaw            Mode 12 : sway + yaw

    Mode 13 : heave + roll          Mode 16 : roll + pitch
    Mode 14 : heave + pitch         Mode 17 : roll + yaw
    Mode 15 : heave + yaw           Mode 18 : pitch + yaw

    Mode 19 : roll + pitch + yaw
    Mode 20 : roll + pitch + surge
    Mode 21 : roll + pitch + sway
    Mode 22 : roll + pitch + heave
    Mode 23 : roll + yaw + surge
    Mode 24 : roll + yaw + sway
    Mode 25 : roll + yaw + heave
    Mode 26 : pitch + yaw + surge
    Mode 27 : pitch + yaw + sway
    Mode 28 : pitch + yaw + heave
'''

# Function to handle keyboard interrupt using the 'Esc' key
def keyboard_int():
    global motion_interrupt
    keyboard.wait("esc")  # 'Esc' key stops motion
    print("Motion interrupted by keyboard.")
    motion_interrupt = True

# Create a CSV file for recording motion data
with open('motion_data.csv', 'w', newline='') as csvfile:
    csv_writer = csv.writer(csvfile)

    # Write a header row to the CSV file
    csv_writer.writerow(["Time", "Roll (rad)", "Pitch (rad)", "Yaw (rad)", "Heave", "Sway", "Surge", "fkRoll (deg)", "fkPitch (deg)", "fkYaw (deg)"])

    # Initialize the ForceSeatMI interface and start motion control
    fsmi = ForceSeatMI()
    fsmi.activate_profile('SDK - Positioning')
    fsmi.begin_motion_control()

    # Initialize the position structure for the motion platform
    position = FSMI_TopTablePositionPhysical()
    position.structSize = sizeof(FSMI_TopTablePositionPhysical)
    position.mask = FSMI_TopTablePositionPhysicalMask()
    position.maxSpeed = 65535

    # If using existing data, load it from a .mat file
    if use_existing_data:
        print("Loading data")
        data = scipy.io.loadmat('EOLOS_Data.mat')
        fields = [field for field in data.keys() if not field.startswith("__")]
        num_days = 1  # Adjust the number of days as needed
        variables = {}

        for var_name in fields:
            var_value = data[var_name]
            if var_name == 'Time' and num_days > 1:
                if isinstance(var_value[0], (np.ndarray, np.generic)) or isinstance(var_value[0], str):
                    cell_data = []
                    for k in range(num_days):
                        if len(cell_data) > 1:
                            last_value = cell_data[-1]
                            time_delta = data[var_name][0, k][1] - data[var_name][0, k][0]
                            new_values = data[var_name][0, k] + last_value + time_delta
                            if not cell_data:
                                cell_data = var_value[0, k]
                            else:
                                cell_data.append(np.concatenate(data[var_name][0, k]))
                        else:
                            if not cell_data:
                                cell_data = var_value[0, k]
                            else:
                                cell_data.append(np.concatenate(data[var_name][0, k]))

                    # Assign the modified values
                    variables[var_name.lower()] = cell_data
                else:
                    # Initialize the variable if it doesn't exist
                    variables[var_name.lower()] = var_value
            else:
                # Handle other variables as before
                if isinstance(var_value[0], (np.ndarray, np.generic)) or isinstance(var_value[0], str):
                    cell_data = []
                    for k in range(num_days):
                        if len(var_value[0, k]) == 1:
                            if not cell_data:
                                cell_data = var_value[0, k]
                            else:
                                cell_data.append(var_value[0, k])
                        else:
                            if not cell_data:
                                cell_data = np.concatenate(var_value[0, k])
                            else:
                                cell_data.append(np.concatenate(var_value[0, k]))

                    # Assign the values to the variables dictionary
                    variables[var_name.lower()] = cell_data

        data_source = "Captured Data"

    else:
        # If not using existing data, generate the motion data
        tstep = 0.05
        pitchAmp = 2.0
        rollAmp = 3.0
        surgeAmp = 100
        heaveAmp = 110
        swayAmp = 100
        period = 10.0
        duration = 500
        t = [i * tstep for i in range(int(duration / tstep))]
        pitch = [(math.pi / 180) * pitchAmp * math.sin((2 * math.pi / period) * i) for i in t]
        roll = [(math.pi / 180) * rollAmp * math.cos((2 * math.pi / period) * i) for i in t]
        surge = [surgeAmp * math.sin((2 * math.pi / period) * i) for i in t]
        heave = [heaveAmp * math.sin((2 * math.pi / period) * i) for i in t]
        sway = [swayAmp * math.sin((2 * math.pi / period) * i) for i in t]
        data_source = "Generated Data"

    # Create and start the keyboard listener thread for interrupt
    keyboard_thread = threading.Thread(target=keyboard_int)
    keyboard_thread.daemon = True
    keyboard_thread.start()

    start_time = time.time()

    # Start the motion control loop
    for i in range(len(t)):
        if motion_interrupt:
            break
        
        current_time = time.time()
        time_elapsed = current_time - start_time
        
        # Synchronize the loop with real-time motion
        while t[i] > time_elapsed:
            current_time = time.time()
            time_elapsed = current_time - start_time
            
        # Retrieve current platform information
        platform_info = FSMI_PlatformInfo()
        if fsmi.get_platform_info_ex(platform_info, sizeof(FSMI_PlatformInfo), 1000):
            fkroll = platform_info.fkRoll
            fkpitch = platform_info.fkPitch
            fkyaw = platform_info.fkYaw
            fkheave = platform_info.fkHeave
            fksway = platform_info.fkSway
            fksurge = platform_info.fkSurge
        else:
            # Default to zero if platform information is unavailable
            fkroll = 0
            fkpitch = 0
            fkyaw = 0
            fkheave = 0
            fksway = 0
            fksurge = 0
            
        # Set the motion mode based on the user's selection
        if motion_mode == 1:  # Surge only
            position.surge = surge[i]
        elif motion_mode == 2:  # Sway only
            position.sway = sway[i]
        elif motion_mode == 3:  # Heave only
            position.heave = heave[i]
        elif motion_mode == 4:  # Roll only
            position.roll = roll[i]
        elif motion_mode == 5:  # Pitch only
            position.pitch = pitch[i]
        elif motion_mode == 6:  # Yaw only
            position.yaw = yaw[i]
        elif motion_mode == 7:  # Surge + Roll
            position.surge = surge[i]
            position.roll = roll[i]
        elif motion_mode == 8:  # Surge + Pitch
            position.surge = surge[i]
            position.pitch = pitch[i]
        elif motion_mode == 9:  # Surge + Yaw
            position.surge = surge[i]
            position.yaw = yaw[i]
        elif motion_mode == 10:  # Sway + Roll
            position.sway = sway[i]
            position.roll = roll[i]
        elif motion_mode == 11:  # Sway + Pitch
            position.sway = sway[i]
            position.pitch = pitch[i]
        elif motion_mode == 12:  # Sway + Yaw
            position.sway = sway[i]
            position.yaw = yaw[i]
        elif motion_mode == 13:  # Heave + Roll
            position.heave = heave[i]
            position.roll = roll[i]
        elif motion_mode == 14:  # Heave + Pitch
            position.heave = heave[i]
            position.pitch = pitch[i]
	elif motion_mode == 15:  # Heave + Yaw
            position.heave = heave[i]
            position.yaw = yaw[i]
        elif motion_mode == 16:  # Roll + Pitch
            position.roll = roll[i]
            position.pitch = pitch[i]
	elif motion_mode == 17:  # Roll + Yaw
            position.roll = roll[i]
            position.yaw = yaw[i]
        elif motion_mode == 18:  # Pitch + Yaw
            position.pitch = pitch[i]
            position.yaw = yaw[i]
        elif motion_mode == 19:  # Roll + Pitch + Yaw
            position.roll = roll[i]
            position.pitch = pitch[i]
            position.yaw = yaw[i]
        elif motion_mode == 20:  # Roll + Pitch + Surge
            position.roll = roll[i]
            position.pitch = pitch[i]
            position.surge = surge[i]
	elif motion_mode == 21:  # Roll + Pitch + Sway
            position.roll = roll[i]
            position.pitch = pitch[i]
            position.sway = sway[i]
	elif motion_mode == 22:  # Roll + Pitch + Heave
            position.roll = roll[i]
            position.pitch = pitch[i]
            position.heave = heave[i]
	elif motion_mode == 23:  # Roll + Yaw + Surge
            position.roll = roll[i]
            position.yaw = yaw[i]
            position.surge = surge[i]
	elif motion_mode == 24:  # Roll + Yaw + Sway
            position.roll = roll[i]
            position.yaw = yaw[i]
            position.sway = sway[i]
	elif motion_mode == 25:  # Roll + Yaw + Heave
            position.roll = roll[i]
            position.yaw = yaw[i]
            position.heave = heave[i]
	elif motion_mode == 26:  # Pitch + Yaw + Surge
            position.pitch = pitch[i]
            position.yaw = yaw[i]
            position.surge = surge[i]
	elif motion_mode == 27:  # Pitch + Yaw + Sway
            position.pitch = pitch[i]
            position.yaw = yaw[i]
            position.sway = sway[i]
	elif motion_mode == 28:  # Pitch + Yaw + Heave
            position.pitch = pitch[i]
            position.yaw = yaw[i]
            position.heave = heave[i]

	#position.roll = 0
        #position.pitch = 0
        #position.yaw = 0
        #position.heave = 0
        #position.sway = 0 #sway[i]
        #position.surge = 0 #surge[i]


        # Send the calculated position to the motion platform
        fsmi.send_top_table_pos_phy(position)

        # Write the data to the CSV file
        curr_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
    	csv_writer.writerow([curr_time, position.roll, position.pitch, position.yaw, position.heave, position.sway, position.surge, fkroll, fkpitch, fkyaw, data_source])

        # Sleep briefly to maintain the loop timing
        time.sleep(0.005)

    # End motion control and clean up resources
    fsmi.end_motion_control()
    fsmi.delete()
