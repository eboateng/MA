from ForceSeatMI import *
import math

fsmi = ForceSeatMI()

fsmi.activate_profile('SDK - Positioning')
fsmi.begin_motion_control()

position = FSMI_TopTablePositionLogical()
position.structSize = sizeof(FSMI_TopTablePositionLogical)
position.mask       = FSMI_TopTablePositionLogicalMask()
position.maxSpeed   = 65535

maxRoll  = 32767
iterator = 0
loop     = 0
maxLoop  = 6

while loop < maxLoop:
    position.roll = int(math.sin(iterator * 3.1415 / 180) * maxRoll)

    fsmi.send_top_table_pos_log(position)

    iterator = iterator + 0.005

    if iterator > 360:
        iterator = 0
        loop = loop + 1

fsmi.end_motion_control()
fsmi.delete()
