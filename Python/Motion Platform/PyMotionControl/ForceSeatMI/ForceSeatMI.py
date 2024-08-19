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
import sys
from sys import platform, maxsize
if any([platform.startswith(os_name) for os_name in ['linux', 'darwin', 'freebsd']]):
    import zugbruecke as ctypes
elif platform.startswith('win'):
    import ctypes
    from winreg import *
else:
    # Handle unsupported platforms
    print('Your platform is not supported')

from .ForceSeatMI_Structs import *

FSMI_FLOAT = float

class FSMI_HandleType(ctypes.Structure):
    pass


def getDllPath() -> str:
    entryName = "InstallationPath"
    is_64bits = sys.maxsize > 2**32
    if any([platform.startswith(os_name) for os_name in ['linux', 'darwin', 'freebsd']]):
        if is_64bits:
            dllName = "ForceSeatMI64.dll"
        else:
            dllName = "ForceSeatMI32.dll"

        path = "dll/" + dllName

    elif platform.startswith('win'):
        if is_64bits:
            reg = "SOFTWARE\\Wow6432Node\\MotionSystems\\ForceSeatPM"
            dllName = "ForceSeatMI64"
        else:
            reg = "SOFTWARE\\MotionSystems\\ForceSeatPM"
            dllName = "ForceSeatMI32"

        key   = OpenKeyEx(HKEY_LOCAL_MACHINE, reg)
        value = QueryValueEx(key, entryName)
        path  = value[0] + dllName
    else:
        # Handle unsupported platforms
        print('Your platform is not supported')
        path = None

    return path


class ForceSeatMI(object):
    def __init__(self):
        path = getDllPath()
        if any([platform.startswith(os_name) for os_name in ['linux', 'darwin', 'freebsd']]):
            ctypes.cdll.LoadLibrary(path)
        else:
            self.lib = CDLL(path)

        # Create API
        self.lib.ForceSeatMI_Create.restype = ctypes.POINTER(FSMI_HandleType)
        self.api = self.lib.ForceSeatMI_Create()

    def delete(self):
        self.lib.ForceSeatMI_Delete(self.api)

    def begin_motion_control(self) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_BeginMotionControl(self.api))
        return (result.value == FSMI_True().value)

    def end_motion_control(self) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_EndMotionControl(self.api))
        return (result.value == FSMI_True().value)

    def get_platform_info_ex(self, platform_info: FSMI_PlatformInfo, platform_info_size: FSMI_UINT32, timeout: FSMI_UINT32) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_GetPlatformInfoEx(self.api, byref(platform_info), platform_info_size, timeout))
        return (result.value == FSMI_True().value)

    def send_telemetry(self, telemetry: FSMI_Telemetry) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_SendTelemetry(self.api, telemetry))
        return (result.value == FSMI_True().value)

    def send_telemetry2(self, telemetry: FSMI_Telemetry, sfx: FSMI_SFX, audio_effects: FSMI_TactileAudioBasedFeedbackEffects) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_SendTelemetry2(self.api, telemetry, sfx, audio_effects))
        return (result.value == FSMI_True().value)

    def send_top_table_pos_log(self, position: FSMI_TopTablePositionLogical) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_SendTopTablePosLog(self.api, position))
        return (result.value == FSMI_True().value)

    def send_top_table_pos_log2(self, position: FSMI_TopTablePositionLogical, sfx: FSMI_SFX, audio_effects: FSMI_TactileAudioBasedFeedbackEffects) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_SendTopTablePosLog2(self.api, position, sfx, audio_effects))
        return (result.value == FSMI_True().value)

    def send_top_table_pos_phy(self, position: FSMI_TopTablePositionPhysical) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_SendTopTablePosPhy(self.api, position))
        return (result.value == FSMI_True().value)

    def send_top_table_pos_phy2(self, position: FSMI_TopTablePositionPhysical, sfx: FSMI_SFX, audio_effects: FSMI_TactileAudioBasedFeedbackEffects) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_SendTopTablePosPhy2(self.api, position, sfx, audio_effects))
        return (result.value == FSMI_True().value)

    def send_top_table_matrix_phy(self, matrix: FSMI_TopTableMatrixPhysical) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_SendTopTableMatrixPhy(self.api, matrix))
        return (result.value == FSMI_True().value)

    def send_top_table_matrix_phy2(self, matrix: FSMI_TopTableMatrixPhysical, sfx: FSMI_SFX, audio_effects: FSMI_TactileAudioBasedFeedbackEffects) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_SendTopTableMatrixPhy2(self.api, matrix, sfx, audio_effects))
        return (result.value == FSMI_True().value)

    def send_tactile_feedback_effects(self, audio_effects: FSMI_TactileAudioBasedFeedbackEffects) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_SendTactileFeedbackEffects(self.api, audio_effects))
        return (result.value == FSMI_True().value)

    def activate_profile(self, profileName: str) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_ActivateProfile(self.api, profileName.encode('utf-8')))
        return (result.value == FSMI_True().value)

    def set_app_id(self, appId: str) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_SetAppID(self.api, appId.encode('utf-8')))
        return (result.value == FSMI_True().value)

    def park(self, parkMode: FSMI_UINT8) -> bool:
        result = FSMI_Bool(self.lib.ForceSeatMI_Park(self.api, parkMode))
        return (result.value == FSMI_True().value)
