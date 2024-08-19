from ForceSeatMI import *
import math

fsmi = ForceSeatMI()

fsmi.activate_profile('SDK - Vehicle Telemetry')
fsmi.begin_motion_control()

telemetry = FSMI_Telemetry()
telemetry.structSize = sizeof(FSMI_Telemetry)
telemetry.mask       = FSMI_TelemetryMask()

platformInfo = FSMI_PlatformInfo()
platformInfo.structSize = sizeof(FSMI_PlatformInfo)

iterator = 0

while iterator < 50:
    telemetry.swayAcceleration = math.sin(iterator) * 20

    fsmi.send_telemetry(telemetry)

    iterator = iterator + 0.00005

    data = fsmi.get_platform_info_ex(platformInfo, sizeof(FSMI_PlatformInfo), 100)

fsmi.end_motion_control()
fsmi.delete()
