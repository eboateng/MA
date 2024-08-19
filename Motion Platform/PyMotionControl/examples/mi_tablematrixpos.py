
from config import *
from time import sleep
import math

from TLCD.helper.load import load_malibu
from archive.coordinates import setMatrix

from ForceSeatMI import *

import serial
ser = serial.Serial('/dev/tty.usbmodem83140101')
ser.flushInput()

if use_malibu:
    pitch_data, roll_data = load_malibu('data/Malibu/2017_03_01_original_results.mat')
    print('Data loaded')
else:
    pitch_data = range(0, 1000)
    roll_data = range(0, 1000)

fsmi = ForceSeatMI()

fsmi.activate_profile('SDK - Positioning')
fsmi.begin_motion_control()

matrix = FSMI_TopTableMatrixPhysical()
matrix.structSize = sizeof(FSMI_TopTableMatrixPhysical)
matrix.mask       = FSMI_TopTableMatrixPhysicalMask()
matrix.maxSpeed   = 65535

platformInfo = FSMI_PlatformInfo()
platformInfo.structSize = sizeof(FSMI_PlatformInfo)

if use_malibu:
    pause = 0.01
else:
    pause = 0.2

step = frequency*pause
value = 0
angle = 0
iterator = 0
pitch = 0
roll = 0

motion_log_roll = list()
motion_log_pitch = list()
motion_log_sensor = list()
try:
    for i in range(len(pitch_data)):
        matrix = setMatrix(matrix, roll, pitch, 0, 0, 0, 0)
        fsmi.send_top_table_matrix_phy(matrix)

        ser_bytes = ser.readline()
        decoded_bytes = float(ser_bytes[0:len(ser_bytes) - 2].decode("utf-8"))
        motion_log_sensor.append(decoded_bytes)

        if use_malibu:
            pitch = pitch_data[i]
            roll = roll_data[i]
        else:
            pitch = maxPitchAngle * math.sin(step*i * 2 * math.pi-math.pi/2.0)
            roll = maxRollAngle * math.sin(step*i * 2 * math.pi)

            angle += step

        fsmi.get_platform_info_ex(platformInfo, sizeof(FSMI_PlatformInfo), 100)

        motion_log_pitch.append(platformInfo.fkPitch)
        motion_log_roll.append(platformInfo.fkRoll)

        iterator = iterator + 1

        sleep(frequency)

except (KeyboardInterrupt, SystemExit):
    print('Keyboard Interrupt')

finally:
    fsmi.park(FSMI_ParkMode_Normal())
    fsmi.end_motion_control()
    fsmi.delete()
    ser.close()
