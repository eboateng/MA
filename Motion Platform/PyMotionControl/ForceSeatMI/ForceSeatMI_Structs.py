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

from .ForceSeatMI_Defines import *

# Check C/C++ header for full fields description
class FSMI_TopTablePositionLogical(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize', FSMI_UINT8),
        ('mask',       FSMI_UINT32),
        ('state',      FSMI_UINT8),
        ('roll',       FSMI_INT16),
        ('pitch',      FSMI_INT16),
        ('yaw',        FSMI_INT16),
        ('heave',      FSMI_INT16),
        ('sway',       FSMI_INT16),
        ('surge',      FSMI_INT16),
        ('maxSpeed',   FSMI_INT16),
        ('triggers',   FSMI_UINT8),
        ('aux',        FSMI_FLOAT * 8)
    ]

# Check C/C++ header for full fields description
class FSMI_TopTablePositionPhysical(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize', FSMI_UINT8),
        ('mask',       FSMI_UINT32),
        ('state',      FSMI_UINT8),
        ('roll',       FSMI_FLOAT),
        ('pitch',      FSMI_FLOAT),
        ('yaw',        FSMI_FLOAT),
        ('heave',      FSMI_FLOAT),
        ('sway',       FSMI_FLOAT),
        ('surge',      FSMI_FLOAT),
        ('maxSpeed',   FSMI_INT16),
        ('triggers',   FSMI_UINT8),
        ('aux',        FSMI_FLOAT * 8)
    ]

# Check C/C++ header for full fields description
class FSMI_TopTableMatrixPhysical(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize',     FSMI_UINT8),
        ('mask',           FSMI_UINT32),
        ('state',          FSMI_UINT8),
        ('transformation', (FSMI_FLOAT * 4) * 4),
        ('maxSpeed',       FSMI_INT16),
        ('triggers',       FSMI_UINT8),
        ('aux',            FSMI_FLOAT * 8)
    ]

# Check C/C++ header for full fields description
class FSMI_PlatformInfo(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize',                   FSMI_UINT8),
        ('timemark',                     FSMI_UINT64),
        ('isConnected',                  FSMI_Bool),
        ('state',                        FSMI_UINT8),
        ('actualMotorPosition',          FSMI_UINT16 * FSMI_MotorsCount()),
        ('actualMotorSpeed',             FSMI_INT32 * FSMI_MotorsCount()),
        ('isThermalProtectionActivated', FSMI_Bool),
        ('worstModuleStatus',            FSMI_UINT8),
        ('worstModuleStatusIndex',       FSMI_UINT8),
        ('coolingSystemMalfunction',     FSMI_Bool),
        ('isKinematicsSupported',        FSMI_Bool),
        ('ikPrecision',                  FSMI_FLOAT * FSMI_MotorsCount()),
        ('ikRecentStatus',               FSMI_Bool),
        ('fkRoll',                       FSMI_FLOAT),
        ('fkPitch',                      FSMI_FLOAT),
        ('fkYaw',                        FSMI_FLOAT),
        ('fkHeave',                      FSMI_FLOAT),
        ('fkSway',                       FSMI_FLOAT),
        ('fkSurge',                      FSMI_FLOAT),
        ('fkRecentStatus',               FSMI_Bool),
        ('requiredMotorPosition',        FSMI_UINT16 * FSMI_MotorsCount()),
        ('fkRoll_deg',                   FSMI_FLOAT),
        ('fkPitch_deg',                  FSMI_FLOAT),
        ('fkYaw_deg',                    FSMI_FLOAT),
        ('fkRollSpeed_deg',              FSMI_FLOAT),
        ('fkPitchSpeed_deg',             FSMI_FLOAT),
        ('fkYawSpeed_deg',               FSMI_FLOAT),
        ('fkHeaveSpeed',                 FSMI_FLOAT),
        ('fkSwaySpeed',                  FSMI_FLOAT),
        ('fkSurgeSpeed',                 FSMI_FLOAT),
        ('fkRollAcc_deg',                FSMI_FLOAT),
        ('fkPitchAcc_deg',               FSMI_FLOAT),
        ('fkYawAcc_deg',                 FSMI_FLOAT),
        ('fkHeaveAcc',                   FSMI_FLOAT),
        ('fkSwayAcc',                    FSMI_FLOAT),
        ('fkSurgeAcc',                   FSMI_FLOAT),
    ]

# Check C/C++ header for full fields description
class FSMI_TactileAudioBasedFeedbackEffects(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize', FSMI_UINT8),
        ('frequency',  FSMI_UINT32 * FSMI_StreamsCount()),
        ('amplitude',  FSMI_FLOAT * FSMI_StreamsCount()),
    ]

# Check C/C++ header for full fields description
class FSMI_Telemetry(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize',          FSMI_UINT8),
        ('mask',                FSMI_UINT32),
        ('state',               FSMI_UINT8),
        ('rpm',                 FSMI_UINT32),
        ('speed',               FSMI_FLOAT),
        ('yaw',                 FSMI_FLOAT),
        ('pitch',               FSMI_FLOAT),
        ('roll',                FSMI_FLOAT),
        ('yawAcceleration',     FSMI_FLOAT),
        ('pitchAcceleration',   FSMI_FLOAT),
        ('rollAcceleration',    FSMI_FLOAT),
        ('yawSpeed',            FSMI_FLOAT),
        ('pitchSpeed',          FSMI_FLOAT),
        ('rollSpeed',           FSMI_FLOAT),
        ('swayAcceleration',    FSMI_FLOAT),
        ('heaveAcceleration',   FSMI_FLOAT),
        ('surgeAcceleration',   FSMI_FLOAT),
        ('swaySpeed',           FSMI_FLOAT),
        ('heaveSpeed',          FSMI_FLOAT),
        ('surgeSpeed',          FSMI_FLOAT),
        ('gasPedalPosition',    FSMI_UINT8),
        ('brakePedalPosition',  FSMI_UINT8),
        ('clutchPedalPosition', FSMI_UINT8),
        ('gearNumber',          FSMI_INT8),
        ('leftSideGroundType',  FSMI_UINT8),
        ('rightSideGroundType', FSMI_UINT8),
        ('collisionForce',      FSMI_FLOAT),
        ('collisionYaw',        FSMI_FLOAT),
        ('collisionPitch',      FSMI_FLOAT),
        ('collisionRoll',       FSMI_FLOAT),
        ('globalPositionX',     FSMI_FLOAT),
        ('globalPositionY',     FSMI_FLOAT),
        ('globalPositionZ',     FSMI_FLOAT),
        ('timeMs',              FSMI_UINT32),
        ('triggers',            FSMI_UINT8),
        ('maxRpm',              FSMI_UINT32),
        ('flags',               FSMI_UINT32),
        ('aux',                 FSMI_FLOAT * 8),
        ('extraYaw',            FSMI_FLOAT),
        ('extraPitch',          FSMI_FLOAT),
        ('extraRoll',           FSMI_FLOAT),
        ('extraSway',           FSMI_FLOAT),
        ('extraHeave',          FSMI_FLOAT),
        ('extraSurge',          FSMI_FLOAT),
    ]

# Check C/C++ header for full fields description
class FSMI_SFX(Structure):
    _pack_   = 1
    _fields_ = [
        ('structSize',          FSMI_UINT8),
        ('effectsCount',        FSMI_UINT8),
        ('effect1Type',         FSMI_UINT8),
        ('effect1Area',         FSMI_UINT8),
        ('effect1Frequency',    FSMI_UINT16),
        ('effect1Amplitude',    FSMI_FLOAT),
        ('effect1Reserved',     FSMI_UINT8 * 8),
        ('effect2Type',         FSMI_UINT8),
        ('effect2Area',         FSMI_UINT16),
        ('effect2Frequency',    FSMI_UINT8),
        ('effect2Amplitude',    FSMI_FLOAT),
        ('effect2Reserved',     FSMI_UINT8 * 8),
        ('effect3Type',         FSMI_UINT8),
        ('effect3Area',         FSMI_UINT16),
        ('effect3Frequency',    FSMI_UINT8),
        ('effect3Amplitude',    FSMI_FLOAT),
        ('effect3Reserved',     FSMI_UINT8 * 8),
        ('effect4Type',         FSMI_UINT8),
        ('effect4Area',         FSMI_UINT16),
        ('effect4Frequency',    FSMI_UINT8),
        ('effect4Amplitude',    FSMI_FLOAT),
        ('effect4Reserved',     FSMI_UINT8 * 8),
    ]
