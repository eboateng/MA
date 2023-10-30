#
# Copyright (C) 2012-2022 MotionSystems
#
# This file is part of ForceSeatMI SDK.
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

from ctypes import *

FSMI_UINT8  = c_uint8
FSMI_INT8   = c_int8
FSMI_UINT32 = c_uint32
FSMI_INT32  = c_int32
FSMI_UINT16 = c_uint16
FSMI_INT16  = c_int16
FSMI_FLOAT  = c_float
FSMI_UINT64 = c_uint64
FSMI_Bool   = c_char
FSMI_Char   = c_char

def FSMI_True():
    return FSMI_Bool(b'\x01')

def FSMI_False():
    return FSMI_Bool(b'\x00')

def FSMI_MotorsCount():
    return 6

def FSMI_SFX_MaxEffectsCount():
    return 4

def FSMI_UserAuxCount():
    return 8

def FSMI_ParkMode_ToCenter():
    return 0

def FSMI_ParkMode_Normal():
    return 1

def FSMI_ParkMode_ForTransport():
    return 2

def FSMI_ModuleStatus_Ok():
    return 0

def FSMI_ModuleStatus_Overheated():
    return 1

def FSMI_ModuleStatus_Communication_Error():
    return 2

def FSMI_ModuleStatus_Config_Error():
    return 3

def FSMI_ModuleStatus_RefPointSwitch_Error():
    return 4

def FSMI_ModuleStatus_Calibration_Error():
    return 5

def FSMI_ModuleStatus_General_Error():
    return 6

def FSMI_ModuleStatus_NotConnected_Error():
    return 7

def FSMI_ModuleStatus_NoPowerSupply_Error():
    return 8

def FSMI_ModuleStatus_FanSpeedTooLow_Error():
    return 9

def FSMI_ModuleStatus_OutOfRange_Error():
    return 10

def FSMI_ModuleStatus_StallDetected_Error():
    return 11

def FSMI_ModuleStatus_EMCY_Error():
    return 12

def FSMI_ModuleStatus_Overcurrent_Error():
    return 13

def FSMI_ModuleStatus_Overloaded_Error():
    return 14


def FSMI_StreamCenter():
    return 0

def FSMI_StreamEngine():
    return 1

def FSMI_StreamFL():
    return 2

def FSMI_StreamFR():
    return 3

def FSMI_StreamRL():
    return 4

def FSMI_StreamRR():
    return 5

def FSMI_StreamCH():
    return 6

def FSMI_StreamsCount():
    return 7

def FSMI_TopTablePositionPhysicalMask():
    return 14

def FSMI_TelemetryMask():
    return 524450

def FSMI_TopTableMatrixPhysicalMask():
    return 14


def FSMI_SFX_EffectType_SinusLevel2():
    return 0

def FSMI_SFX_EffectType_SinusLevel3():
    return 1

def FSMI_SFX_EffectType_SinusLevel4():
    return 2

def FSMI_SFX_AreaFlags_FrontLeft():
    return 1

def FSMI_SFX_AreaFlags_FrontRight():
    return 2

def FSMI_SFX_AreaFlags_RearLeft():
    return 4

def FSMI_SFX_AreaFlags_RearRight():
    return 8

def FSMI_SFX_AreaFlags_Aux1():
    return 16

def FSMI_SFX_AreaFlags_Aux2():
    return 32
