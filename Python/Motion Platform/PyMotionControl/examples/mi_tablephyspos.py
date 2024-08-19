
from ForceSeatMI import *
import math

fsmi = ForceSeatMI()

fsmi.activate_profile('SDK - Positioning')
fsmi.begin_motion_control()

position = FSMI_TopTablePositionPhysical()
position.structSize = sizeof(FSMI_TopTablePositionPhysical)
position.mask       = FSMI_TopTablePositionPhysicalMask()
position.maxSpeed   = 65535

maxRoll  = math.radians(10)
iterator = 0

while iterator < 100:
    position.roll = math.sin(iterator) * maxRoll
    #position.pitch = math.sin(iterator) * maxRoll
    fsmi.send_top_table_pos_phy(position)

    iterator = iterator + 0.000001

fsmi.end_motion_control()
fsmi.delete()
