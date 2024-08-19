#
# Copyright (C) 2012-2020 Motion Systems
#
# This file is part of ForceSeat motion system.
#
# www.motionsystems.eu
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

from .ForceSeatDI_Structs import *

import sys

# import ctypes
# ctype is replaced by zugbruecke on osx
if any([sys.platform.startswith(os_name) for os_name in ['linux', 'darwin', 'freebsd']]):
    import zugbruecke as ctypes
elif sys.platform.startswith('win'):
    import ctypes
else:
    # Handle unsupported platforms
    print('Your platform is not supported')


class FSDI_HandleType(ctypes.Structure):
    pass


class ForceSeatDI(object):
    def __init__(self):
        is_64bits = sys.maxsize > 2**32
        if is_64bits:
            self.lib = CDLL("ForceSeatDI/ForceSeatDI64")
        else:
            self.lib = CDLL('ForceSeatDI/ForceSeatDI32')

        # Create API
        self.lib.ForceSeatDI_Create.restype = ctypes.POINTER(FSDI_HandleType)
        self.api = self.lib.ForceSeatDI_Create()

    def delete(self):
        self.lib.ForceSeatDI_Delete(self.api)

    def connect_to_usb_device(self, name: str = "", serialNumber: str = "") -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_ConnectToUsbDevice(self.api, name, serialNumber))
        return (result.value == FSDI_True().value)

    def connect_to_network_device(self, ipAddress: str) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_ConnectToNetworkDevice(self.api, ipAddress.encode('utf-8')))
        return (result.value == FSDI_True().value)

    def test_connection(self, is_connected: FSDI_Bool) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_TestConnection(self.api, byref(is_connected)))
        return (result.value == FSDI_True().value)

    def park(self, park_mode: c_uint8) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_Park(self.api, park_mode))
        return (result.value == FSDI_True().value)

    def send_top_table_pos_log(self, position: FSDI_TopTablePositionLogical) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_SendTopTablePosLog(self.api, byref(position)))
        return (result.value == FSDI_True().value)

    def send_top_table_pos_log2(self, position: FSDI_TopTablePositionLogical, sfx: FSDI_SFX) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_SendTopTablePosLog(self.api, byref(position), byref(sfx)))
        return (result.value == FSDI_True().value)

    def send_top_table_pos_phy(self, position: FSDI_TopTablePositionPhysical) -> bool:
         result = FSDI_Bool(self.lib.ForceSeatDI_SendTopTablePosPhy(self.api, byref(position)))
         return (result.value == FSDI_True().value)

    def send_top_table_pos_phy2(self, position: FSDI_TopTablePositionPhysical, sfx: FSDI_SFX) -> bool:
         result = FSDI_Bool(self.lib.ForceSeatDI_SendTopTablePosPhy2(self.api, byref(position), byref(sfx)))
         return (result.value == FSDI_True().value)

    def send_top_table_matrix_phy(self, matrix: FSDI_TopTableMatrixPhysical) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_SendTopTableMatrixPhy(self.api, byref(matrix)))
        return (result.value == FSDI_True().value)

    def send_top_table_matrix_phy2(self, matrix: FSDI_TopTableMatrixPhysical, sfx: FSDI_SFX) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_SendTopTableMatrixPhy2(self.api, byref(matrix), byref(sfx)))
        return (result.value == FSDI_True().value)

    def send_actuators_pos_log(self, position: FSDI_RequiredActuatorsPositionLogical) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_SendActuatorsPosLog(self.api, byref(position)))
        return (result.value == FSDI_True().value)

    def send_actuators_pos_log2(self, position: FSDI_RequiredActuatorsPositionLogical, sfx: FSDI_SFX) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_SendActuatorsPosLog2(self.api, byref(position), byref(sfx)))
        return (result.value == FSDI_True().value)

    def get_platform_info(self, platformInfo: FSDI_PlatformInfo) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_GetPlatformInfo(self.api, byref(platformInfo)))
        return (result.value == FSDI_True().value)

    def get_actuators_pos_log(self, position: FSDI_ActualActuatorsPositionLogical) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_GetActuatorsPosLog(self.api, byref(position)))
        return (result.value == FSDI_True().value)

    def get_license_status(self, is_valid: FSDI_Bool) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_GetLicenseStatus(self.api, byref(is_valid)))
        return (result.value == FSDI_True().value)

    def get_top_table_pos_phy(self, position: FSDI_ActualTopTablePositionPhysical) -> bool:
        result = FSDI_Bool(self.lib.ForceSeatDI_GetTopTablePosPhy(self.api, byref(position)))
        return (result.value == FSDI_True().value)

    def get_recent_error_code(self) -> FSDI_INT32:
        return self.lib.ForceSeatDI_GetRecentErrorCode(self.api)
