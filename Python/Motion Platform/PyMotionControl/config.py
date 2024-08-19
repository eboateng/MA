# Settings file
from sys import platform

# Serial port
if any([platform.startswith(os_name) for os_name in ['linux', 'darwin', 'freebsd']]):
    port = r'/dev/tty.usbmodem1442201'
else:
    port = "COM8"

baudrate = 112500

# Inputs
use_malibu = False
malibu_path = 'Z:/Nextcloud/Masterarbeit/Daten/Malibu_FINO3'

# Outputs
data_output = 'Z:/Nextcloud/Masterarbeit/Daten/Tests'
artemis_columns = ['timestamp', 'rtcTime',
                   'aX', 'aY', 'aZ', 'gX', 'gY', 'gZ', 'mX', 'mY', 'mZ', 'imu_degC',
                   'distance_1_mm', 'distance_1_status',
                   'distance_2_mm', 'distance_2_status',
                   'distance_3_mm', 'distance_3_status',
                   'pressure_Pa', 'humidity_percent', 'altitude_m', 'temp_degC',
                   'output_Hz']

# Motion
trajectory_type = 'swing'
duration = 120# seconds
pitch = {'amplitude': 10.0, 'period': 4.0, 'phase': 0.0}  # degree
roll = {'amplitude': 0.0, 'period': 3.8, 'phase': 0.0, 'range': 10.0}  # degree
# yaw = {'amplitude': 10.0, 'period': 10.0, 'phase': 0.0}
surge = {'amplitude': 0.0*60, 'period': 3.4, 'phase': 0.0}
sway = {'amplitude': 0.0*60, 'period': 8.3, 'phase': 0.0}
heave = {'amplitude': 0.0*60, 'period': 7.1, 'phase': 0.0}

# Center of rotation
correct_cor = True
center = {'x': 0.0, 'y': 0.0, 'z': 0.0}

# motion control
frequency = 10  # Hz




