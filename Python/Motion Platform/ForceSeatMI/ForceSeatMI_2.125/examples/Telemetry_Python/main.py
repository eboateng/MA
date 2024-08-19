from ForceSeatMI import *
import math
import time

fsmi = ForceSeatMI()
fsmi.activate_profile('SDK - Vehicle Telemetry ACE')
fsmi.begin_motion_control()

telemetry = FSMI_TelemetryACE()
telemetry.structSize = sizeof(FSMI_TelemetryACE)
telemetry.mask       = FSMI_TelemetryMask()

platformInfo = FSMI_PlatformInfo()
platformInfo.structSize = sizeof(FSMI_PlatformInfo)

iterator = 0

while iterator < 50:
    telemetry.bodyLinearAcceleration.forward = math.sin(iterator) * 100

    fsmi.send_telemetry_ace(telemetry)
    
    iterator = iterator + 0.05

    fsmi.get_platform_info_ex(platformInfo, sizeof(FSMI_PlatformInfo), 100)

    time.sleep(0.005)

fsmi.end_motion_control()
fsmi.delete()
