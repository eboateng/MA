import time
import math
import csv
from ForceSeatMI import *
from datetime import datetime
import threading
import keyboard  

motion_interrupt = False
use_existing_data = False  # Set to True to use captured data
    
    
def keyboard_int():
    global motion_interrupt
    keyboard.wait("esc")  # 'Esc' key stops motion
    print("Motion interrupted by keyboard.")
    motion_interrupt = True

# Create a CSV file for recording motion data
with open('motion_data.csv', 'w', newline='') as csvfile:
    csv_writer = csv.writer(csvfile)

    # Write a header row
    csv_writer.writerow(["Time", "Roll (rad)", "Pitch (rad)", "Yaw (rad)", "Heave", "Sway", "Surge", "fkRoll (deg)", "fkPitch (deg)", "fkYaw (deg)"])

    fsmi = ForceSeatMI()
    fsmi.activate_profile('SDK - Positioning')
    fsmi.begin_motion_control()

    position = FSMI_TopTablePositionPhysical()
    position.structSize = sizeof(FSMI_TopTablePositionPhysical)
    position.mask = FSMI_TopTablePositionPhysicalMask()
    position.maxSpeed = 65535

    
    if use_existing_data:
        # load data from .mat file
        mat_file_path = "your_data.mat"  
        timestamps, roll, pitch, yaw, heave, sway, surge = load_data_from_mat(mat_file_path)
        data_source = "Captured Data" 

        # Ensure that timestamps start at zero
        timestamps = [t - timestamps[0] for t in timestamps]
        
    else:
        tstep = 0.05
        pitchAmp = 12.0
        rollAmp = 6.0
        period = 10.0
        duration = 500
        t = [i * tstep for i in range(int(duration / tstep))]
        pitch = [(math.pi / 180) * pitchAmp * math.sin((2 * math.pi / period) * i) for i in t]
        roll = [(math.pi / 180) * rollAmp * math.cos((2 * math.pi / period) * i) for i in t]
        data_source = "Generated Data"

    # Create and start the keyboard listener thread
    keyboard_thread = threading.Thread(target=keyboard_int)
    keyboard_thread.daemon = True
    keyboard_thread.start()

    start_time = time.time()
    for i in range(len(t)):
        if motion_interrupt:
            break

        current_time = time.time()
        time_elapsed = current_time - start_time

        while t[i] > time_elapsed:
            current_time = time.time()
            time_elapsed = current_time - start_time
            
        platform_info = FSMI_PlatformInfo()
        if fsmi.get_platform_info_ex(platform_info, sizeof(FSMI_PlatformInfo), 1000):
            fkroll = platform_info.fkRoll
            fkpitch = platform_info.fkPitch
            fkyaw = platform_info.fkYaw
            fkheave = platform_info.fkHeave
            fksway = platform_info.fkSway
            fksurge = platform_info.fkSurge
        else:
            fkroll = 0
            fkpitch = 0
            fkyaw = 0
            fkheave = 0
            fksway = 0
            fksurge = 0

        position.roll = roll[i]
        position.pitch = pitch[i]
        position.yaw = 0  # You can adjust the yaw value if needed
        position.heave = 0
        position.sway = 0
        position.surge = 0

        fsmi.send_top_table_pos_phy(position)

        # Write the data to the CSV file
        curr_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
        csv_writer.writerow([curr_time, position.roll, position.pitch, position.yaw, position.heave, position.sway, position.surge, fkroll, fkpitch, fkyaw, data_source])

        time.sleep(0.005)

    fsmi.end_motion_control()
    fsmi.delete()
