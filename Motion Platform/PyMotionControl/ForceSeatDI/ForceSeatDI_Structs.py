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

from .ForceSeatDI_Defines import *

FSDI_ParkMode_Normal       = 0
FSDI_ParkMode_ToCenter     = 1
FSDI_ParkMode_ForTransport = 2


# Check C/C++ header for full fields description
class FSDI_TopTablePositionLogical(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize', FSDI_UINT8),
        ('mask',       FSDI_UINT32),
        ('pause',      FSDI_Bool),
        ('roll',       FSDI_INT16),
        ('pitch',      FSDI_INT16),
        ('yaw',        FSDI_INT16),
        ('heave',      FSDI_INT16),
        ('sway',       FSDI_INT16),
        ('surge',      FSDI_INT16),
        ('maxSpeed',   FSDI_UINT16)
    ]

# Check C/C++ header for full fields description
class FSDI_TopTablePositionPhysical(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize', FSDI_UINT8),
        ('mask',       FSDI_UINT32),
        ('pause',      FSDI_Bool),
        ('roll',       FSDI_FLOAT),
        ('pitch',      FSDI_FLOAT),
        ('yaw',        FSDI_FLOAT),
        ('heave',      FSDI_FLOAT),
        ('sway',       FSDI_FLOAT),
        ('surge',      FSDI_FLOAT),
        ('maxSpeed',   FSDI_UINT16),
        ('strategy',   FSDI_UINT8),
    ]

# Check C/C++ header for full fields description
class FSDI_TopTableMatrixPhysical(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize',     FSDI_UINT8),
        ('mask',           FSDI_UINT32),
        ('pause',          FSDI_Bool),
        ('transformation', (FSDI_FLOAT * 4) * 4),
        ('maxSpeed',       FSDI_UINT16)
    ]

# Check C/C++ header for full fields description
class FSDI_RequiredActuatorsPositionLogical(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize',       FSDI_UINT8),
        ('mask',             FSDI_UINT32),
        ('pause',            FSDI_Bool),
        ('actuatorPosition', FSDI_UINT16 * FSDI_MotorsCount()),
        ('maxSpeed',         FSDI_UINT16)
    ]

# Check C/C++ header for full fields description
class FSDI_PlatformInfo(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize',                   FSDI_UINT8),
        ('state',                        FSDI_UINT32),
        ('isThermalProtectionActivated', FSDI_Bool),
        ('coolingSystemMalfunction',     FSDI_Bool),
        ('moduleStatus',                 FSDI_UINT8 * FSDI_MotorsCount()),
        ('serialNumber',                 FSDI_UINT8 * FSDI_SerialNumberBytesLength()),
    ]

# Check C/C++ header for full fields description
class FSDI_ActualActuatorsPositionLogical(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize',                    FSDI_UINT8),
        ('state',                         FSDI_UINT32),
        ('actualMotorPosition',           FSDI_UINT16 * FSDI_MotorsCount()),
        ('actualMotorSpeed',              FSDI_INT32  * FSDI_MotorsCount()),
        ('requiredMotorPosition',         FSDI_UINT16 * FSDI_MotorsCount()),
        ('maxAllowedMotorSpeed_obsolete', FSDI_UINT16 * FSDI_MotorsCount())
    ]

# Check C/C++ header for full fields description
class FSDI_ActualTopTablePositionPhysical(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize', FSDI_UINT8),
        ('state',      FSDI_UINT32),
        ('roll',       FSDI_FLOAT),
        ('pitch',      FSDI_FLOAT),
        ('yaw',        FSDI_FLOAT),
        ('heave',      FSDI_FLOAT),
        ('sway',       FSDI_FLOAT),
        ('surge',      FSDI_FLOAT)
    ]

# Check C/C++ header for full fields description
class FSDI_SFX(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize',          FSDI_UINT8),
        ('effectsCount',        FSDI_UINT8),
        ('effect1Type',         FSDI_UINT8),
        ('effect1Area',         FSDI_UINT8),
        ('effect1Frequency',    FSDI_UINT16),
        ('effect1Amplitude',    FSDI_FLOAT),
        ('effect1Reserved',     FSDI_UINT8 * 8),
        ('effect2Type',         FSDI_UINT8),
        ('effect2Area',         FSDI_UINT16),
        ('effect2Frequency',    FSDI_UINT8),
        ('effect2Amplitude',    FSDI_FLOAT),
        ('effect2Reserved',     FSDI_UINT8 * 8),
        ('effect3Type',         FSDI_UINT8),
        ('effect3Area',         FSDI_UINT16),
        ('effect3Frequency',    FSDI_UINT8),
        ('effect3Amplitude',    FSDI_FLOAT),
        ('effect3Reserved',     FSDI_UINT8 * 8),
        ('effect4Type',         FSDI_UINT8),
        ('effect4Area',         FSDI_UINT16),
        ('effect4Frequency',    FSDI_UINT8),
        ('effect4Amplitude',    FSDI_FLOAT),
        ('effect4Reserved',     FSDI_UINT8 * 8),
    ]
