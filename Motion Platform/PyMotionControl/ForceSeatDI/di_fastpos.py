

from ForceSeatDI import *
import math


def main():
    fsdi = ForceSeatDI()

    result = fsdi.connect_to_usb_device()
    print("connect_to_usb_device       = ", result)

    if result:
        is_connected = FSDI_True()
        result = fsdi.test_connection(is_connected)
        print("test_connection (after)     = ", is_connected.value == FSDI_True().value)

        if is_connected:
            is_valid = FSDI_True()
            result = fsdi.get_license_status(is_valid)
            print("get_license_status          = ", is_valid.value == FSDI_True().value)

            is_valid = FSDI_True()

            if is_valid:
                iterator = 0
                maxRoll  = 32767
                position            = FSDI_TopTablePositionLogical()
                position.structSize = sizeof(FSDI_TopTablePositionLogical)
                position.maxSpeed   = 65535
                position.pause      = FSDI_False()
                position.mask       = FSDI_TopTablePositionLogicalMask()

                platformInfo = FSDI_PlatformInfo()
                platformInfo.structSize = sizeof(FSDI_PlatformInfo)

                actuatorsPosition = FSDI_ActualActuatorsPositionLogical()
                actuatorsPosition.structSize = sizeof(FSDI_ActualActuatorsPositionLogical)

                topTablePositionPhysical= FSDI_ActualTopTablePositionPhysical()
                topTablePositionPhysical.structSize = sizeof(FSDI_ActualTopTablePositionPhysical)

                while (iterator < 2000):
                    position.roll = int(math.sin(iterator * 3.1415 / 180) * maxRoll)
                    fsdi.send_top_table_pos_log(position)
                    iterator = iterator + 1

                    fsdi.get_platform_info(platformInfo)
                    fsdi.get_actuators_pos_log(actuatorsPosition)
                    fsdi.get_top_table_pos_phy(topTablePositionPhysical)
                    errorCode = fsdi.get_recent_error_code()
                    if errorCode:
                        print(f"Error: {errorCode}")

    fsdi.park(0)
    fsdi.delete()
    print("Motion finished")

if __name__ == "__main__":
    main()
