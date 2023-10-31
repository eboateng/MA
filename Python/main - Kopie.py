from ForceSeatMI import *
import math
import time

fsmi = ForceSeatMI()
fsmi.activate_profile('SDK - Positioning')
fsmi.begin_motion_control()

position = FSMI_TopTablePositionPhysical()
position.structSize = sizeof(FSMI_TopTablePositionPhysical)
position.mask       = FSMI_TopTablePositionPhysicalMask()
position.maxSpeed   = 65535

max_roll_rad = math.radians(10);
iterator = 0

while iterator < 50:
    position.roll = math.sin(iterator) * max_roll_rad;

    fsmi.send_top_table_pos_phy(position)

    iterator = iterator + 0.01

    time.sleep(0.005)

fsmi.end_motion_control()
fsmi.delete()
