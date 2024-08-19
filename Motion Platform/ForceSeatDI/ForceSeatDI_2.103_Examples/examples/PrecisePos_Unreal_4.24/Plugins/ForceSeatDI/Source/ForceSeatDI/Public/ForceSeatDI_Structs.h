/*
 * Copyright (C) 2012-2020 Motion Systems
 * 
 * This file is part of ForceSeat motion system.
 *
 * www.motionsystems.eu
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef FORCE_SEAT_DI_STRUCTS_H
#define FORCE_SEAT_DI_STRUCTS_H

#include "ForceSeatDI_Defines.h"

/*
 * List of possible module status values
 */
typedef enum FSDI_ModuleStatus
{
	FSDI_ModuleStatus_Ok                    = 0,
	FSDI_ModuleStatus_Overheated            = 1,
	FSDI_ModuleStatus_Communication_Error   = 2,
	FSDI_ModuleStatus_Config_Error          = 3,
	FSDI_ModuleStatus_RefPointSwitch_Error  = 4,
	FSDI_ModuleStatus_Calibration_Error     = 5,
	FSDI_ModuleStatus_General_Error         = 6,
	FSDI_ModuleStatus_NotConnected_Error    = 7,
	FSDI_ModuleStatus_NoPowerSupply_Error   = 8,
	FSDI_ModuleStatus_FanSpeedTooLow_Error  = 9,
	FSDI_ModuleStatus_OutOfRange_Error      = 10,
	FSDI_ModuleStatus_StallDetected_Error   = 11,
	FSDI_ModuleStatus_EMCY_Error            = 12,
	FSDI_ModuleStatus_Overcurrent_Error     = 13,
	FSDI_ModuleStatus_Overloaded_Error      = 14
} FSDI_ModuleStatus;

/*
 * Possible operating states of the motion platform.
 */
typedef enum FSDI_State
{
	FSDI_State_AnyPaused            = 1 << 0, // 0b00000001
	FSDI_State_ParkingCompleted     = 1 << 1, // 0b00000010
	FSDI_State_Offline              = 1 << 2, // 0b00000100

	// Only one park mode is valid at given time.
	// Greater parking mode value (number) = greater priority.
	// It means that transport parking wins over normal parking
	// and normal parking wins over parking to center.
	FSDI_State_ParkModeMask         = 0xE0,   // 0b11100000
	FSDI_State_NoParking            = 0x00,   // 0b00000000
	FSDI_State_SoftParkToCenter     = 0x20,   // 0b00100000
	FSDI_State_SoftParkNormal       = 0x40,   // 0b01000000
	FSDI_State_SoftParkForTransport = 0x60,   // 0b01100000
	FSDI_State_HardParkToCenter     = 0x80,   // 0b10000000
	FSDI_State_HardParkNormal       = 0xA0,   // 0b10100000
	FSDI_State_HardParkForTransport = 0xC0,   // 0b11000000
	FSDI_State_Reserved             = 0xE0,   // 0b11100000
} FSDI_State;

/*
 * Possible parking modes for the motion platform.
 */
typedef enum FSDI_ParkMode
{
	FSDI_ParkMode_Normal       = 0,
	FSDI_ParkMode_ToCenter     = 1,
	FSDI_ParkMode_ForTransport = 2
} FSDI_ParkMode;

/*
 * Strategy for finding reachable position from request position. It is used by Inverse Kinematics module.
 */
typedef enum FSDI_Strategy
{
	FSDI_Strategy_FullMatch = 0, // Go to required position or report an error if the position is outside work envelop
	FSDI_Strategy_BestMatch = 1  // Go to closest reachable position
} FSDI_Strategy;

typedef enum FSDI_SFX_EffectType
{
	// Effect is appended to the required position before trajectory generator uses the position.
	// It means that this effect affects state of the adaptive algorithms which are used to adjust motion platform
	// operation parameters to the signal dynamics. This effect might be also less pronounced if trajectory generator
	// is configured to use long acceleration ramps.
	// It is supported only by "PS" and "QS" motion platforms.
	FSDI_SFX_EffectType_SinusLevel2 = 0,

	// Effect is appended to the output (position) of the trajectory generator. It is NOT affected by the
	// trajectory generator configuration and it does NOT affect the adaptive algorithms. Result of this effect
	// might be different depending on the actuator hardware configuration (length/pitch).
	// It is supported only by "QS" motion platforms.
	FSDI_SFX_EffectType_SinusLevel3 = 1,

	// Effect is appended to the final motor torque. It is NOT affected by the trajectory generator and other
	// control subsystems. Result of this effect might be different depending on the actuator hardware
	// configuration (length/pitch).
	// It is supported only by "QS" motion platforms.
	FSDI_SFX_EffectType_SinusLevel4 = 2
} FSDI_SFX_EffectType;

typedef enum FSDI_SFX_AreaFlags
{
	// Effect is generated on the actuator(s) assigned to the front-left area of the cockpit.
	FSDI_SFX_AreaFlags_FrontLeft  = 1 << 0,

	// Effect is generated on the actuator(s) assigned to the front-right area of the cockpit.
	FSDI_SFX_AreaFlags_FrontRight = 1 << 1,

	// Effect is generated on the actuator(s) assigned to the rear-left area of the cockpit.
	FSDI_SFX_AreaFlags_RearLeft   = 1 << 2,

	// Effect is generated on the actuator(s) assigned to the rear-right area of the cockpit.
	FSDI_SFX_AreaFlags_RearRight  = 1 << 3,

	// Effect is generated on auxiliary actuator, e.g. seat belt tensioner.
	FSDI_SFX_AreaFlags_Aux1       = 1 << 4,

	// Effect is generated on auxiliary actuator, e.g. seat belt tensioner.
	FSDI_SFX_AreaFlags_Aux2       = 1 << 5
} FSDI_SFX_AreaFlags;

#pragma pack(push, 1)

/*
 * This structure defines position of top frame (table) in logical units (percents).
 * It does not use Inverse Kinematics module so rotation and movements are not always linear.
 */
typedef struct FSDI_PACKED FSDI_TopTablePositionLogical
{
	// Put here sizeof(FSDI_TopTablePositionLogical).
	// NOTE: This field is mandatory.
	FSDI_UINT8  structSize;

	// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
	FSDI_UINT32 mask;

	// NOTE: If the field is provided, set FSDI_BIT_PAUSE in the mask.
	FSDI_Bool   pause;

	// NOTE: If below fields are provided, set FSDI_BIT_POSITION in the mask.
	FSDI_INT16  roll;       // -32k max left, +32k max right
	FSDI_INT16  pitch;      // -32k max rear, +32k max front
	FSDI_INT16  yaw;        // -32k max left, +32k max right
	FSDI_INT16  heave;      // -32k max bottom, +32k max top
	FSDI_INT16  sway;       // -32k max left, +32k max right
	FSDI_INT16  surge;      // -32k max rear, +32k max front

	// 0 to 64k, actual speed is not always equal to max speed due to ramps.
	// NOTE: If below fields are provided, set FSDI_BIT_MAX_SPEED in the mask.
	FSDI_UINT16 maxSpeed;

} FSDI_TopTablePositionLogical;

/*
 * This structure defines position of top frame (table) in physical units (rad, mm).
 * It uses Inverse Kinematics module and it might not be supported by all motion platforms.
 * By default FullMatch strategy is used.
 */
typedef struct FSDI_PACKED FSDI_TopTablePositionPhysical
{
	// Put here sizeof(FSDI_TopTablePositionPhysical).
	// NOTE: This field is mandatory.
	FSDI_UINT8  structSize;

	// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
	FSDI_UINT32 mask;

	// NOTE: If the field is provided, set FSDI_BIT_PAUSE in the mask.
	FSDI_Bool   pause;

	// NOTE: If below fields are provided, set FSDI_BIT_POSITION in the mask.
	FSDI_FLOAT  roll;       // in radians, roll  < 0 = left,  roll > 0  = right
	FSDI_FLOAT  pitch;      // in radians, pitch < 0 = front, pitch > 0 = rear
	FSDI_FLOAT  yaw;        // in radians, yaw   < 0 = right, yaw > 0   = left
	FSDI_FLOAT  heave;      // in mm, heave < 0 - down, heave > 0 - top
	FSDI_FLOAT  sway;       // in mm, sway  < 0 - left, sway  > 0 - right
	FSDI_FLOAT  surge;      // in mm, surge < 0 - rear, surge > 0 - front

	// 0 to 64k, actual speed is not always equal to max speed due to ramps.
	// NOTE: If below fields are provided, set FSDI_BIT_MAX_SPEED in the mask.
	FSDI_UINT16 maxSpeed;

	// One of FSDI_Strategy.
	// NOTE: If below fields are provided, set FSDI_BIT_STRATEGY in the mask.
	FSDI_UINT8  strategy;  

} FSDI_TopTablePositionPhysical;

/*
 * This structure defines position of top frame (table) in physical units (rad, mm) by specifing transformation matrix.
 * It uses Inverse Kinematics module and it is dedicated for 6DoF motion platforms.
 * If matrix transformation is specified, the Inverse Kinematics module always uses FullMatch strategy.
 */
typedef struct FSDI_PACKED FSDI_TopTableMatrixPhysical
{
	// Put here sizeof(FSDI_TopTableMatrixPhysical).
	// NOTE: This field is mandatory.
	FSDI_UINT8  structSize;

	// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
	FSDI_UINT32 mask;

	// NOTE: If the field is provided, set FSDI_BIT_PAUSE in the mask.
	FSDI_Bool   pause;

	// 3D transformation matrix
	// OFFSET (in mm):
	// x axis = left-right movement, sway,  x < 0 - left, x > 0 - right
	// y axis = rear-front movement, surge, y < 0 - rear, y > 0 - front
	// z axis = down-top movement,   heave, z < 0 - down, z > 0 - top
	// 
	// ROTATION (in radians):
	// x axis, pitch = x < 0 = front, x > 0 = rear
	// y axis, roll = y < 0  = left,  y > 0 = right
	// z axis, yaw  = z < 0  = right, z > 0 = left
	//
	// NOTE: If below fields are provided, set FSDI_BIT_TRANSFORMATION in the mask.
	FSDI_FLOAT  transformation[4][4];

	// 0 to 64k, actual speed is not always equal to max speed due to ramps.
	// NOTE: If below fields are provided, set FSDI_BIT_MAX_SPEED in the mask.
	FSDI_UINT16 maxSpeed;

	// One of FSDI_Strategy.
	// NOTE: If below fields are provided, set FSDI_BIT_STRATEGY in the mask.
	FSDI_UINT8  strategy;  

} FSDI_TopTableMatrixPhysical;

/*
 * This structure defines position of actuators arms in logical units
 */
typedef struct FSDI_PACKED FSDI_RequiredActuatorsPositionLogical
{
	// Put here sizeof(FSDI_RequiredActuatorsPositionLogical).
	// NOTE: This field is mandatory.
	FSDI_UINT8  structSize;

	// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
	FSDI_UINT32 mask;

	// NOTE: If the field is provided, set FSDI_BIT_PAUSE in the mask.
	FSDI_Bool   pause;

	// 0 min, +64k max
	// NOTE: If below fields are provided, set FSDI_BIT_POSITION in the mask.
	FSDI_UINT16 actuatorPosition[FSDI_MotorsCount]; 

	// 0 to 64k, actual speed is not always equal to max speed due to ramps.
	// NOTE: If below fields are provided, set FSDI_BIT_MAX_SPEED in the mask.
	FSDI_UINT16 maxSpeed;

} FSDI_RequiredActuatorsPositionLogical;

/*
 * Actual platform status received from the controller
 */
typedef struct FSDI_PACKED FSDI_PlatformInfo
{
	// Set this to sizeof(FSDI_PlatformInfo) before calling 'get' function.
	FSDI_UINT8  structSize; 

	// Check FSDI_State for bit field explanation.
	FSDI_UINT32 state;

	FSDI_Bool   isThermalProtectionActivated;    // Global thermal protection status.
	FSDI_Bool   coolingSystemMalfunction;        // Global cooling system status.
	FSDI_UINT8  moduleStatus[FSDI_MotorsCount];  // Status of modules (actuator or node) - one of FSDI_ModuleStatus

	FSDI_UINT8  serialNumber[FSDI_SerialNumberBytesLength];
} FSDI_PlatformInfo;

/*
 * Actual actuators position received from the controller in logical units
 */
typedef struct FSDI_PACKED FSDI_ActualActuatorsPositionLogical
{
	// Set this to to sizeof(FSDI_ActualActuatorsPositionLogical) before calling 'get' function.
	FSDI_UINT8  structSize; 

	// Check FSDI_State for bit field explanation.
	FSDI_UINT32 state;

	FSDI_UINT16 actualMotorPosition[FSDI_MotorsCount];
	FSDI_INT32  actualMotorSpeed[FSDI_MotorsCount];

	FSDI_UINT16 requiredMotorPosition[FSDI_MotorsCount];
	FSDI_UINT16 maxAllowedMotorSpeed_obsolete[FSDI_MotorsCount]; // this field is not used anymore
} FSDI_ActualActuatorsPositionLogical;

/*
 * This structure defines position of top frame (table) in physical units (rad, mm).
 * It is calculated by forward kinematics from actual actuators position.
 */
typedef struct FSDI_PACKED FSDI_ActualTopTablePositionPhysical
{
	// Set this to to sizeof(FSDI_ActualTopTablePositionPhysical) before calling 'get' function.
	FSDI_UINT8  structSize; 

	// Check FSDI_State for bit field explanation
	FSDI_UINT32 state; 

	FSDI_FLOAT  roll;   // in radians, roll  < 0 = left,  roll > 0  = right
	FSDI_FLOAT  pitch;  // in radians, pitch < 0 = front, pitch > 0 = rear
	FSDI_FLOAT  yaw;    // in radians, yaw   < 0 = right, yaw > 0   = left
	FSDI_FLOAT  heave;  // in mm, heave < 0 - down, heave > 0 - top
	FSDI_FLOAT  sway;   // in mm, sway  < 0 - left, sway  > 0 - right
	FSDI_FLOAT  surge;  // in mm, surge < 0 - rear, surge > 0 - front
} FSDI_ActualTopTablePositionPhysical;

typedef struct FSDI_PACKED FSDI_SFX_Effect
{
	// Effect type. It is one of FSDI_SFX_EffectType.
	FSDI_UINT8 type;

	// Area where the effect should be generated. It is combination of FSDI_SFX_AreaFlags.
	FSDI_UINT16 area;

	// Frequency (Hz) of the effect, from 0 to 255. Not all effects support all frequencies.
	// - SinusL2: 0 to 100
	// - SinusL3: 0 to 100
	// - SinusL4: 0 to 100
	FSDI_UINT8 frequency;

	// Amplitude of the effect, from -1 to 1. Not all effects support full amplitudes.
	// - SinusL2: -0.12 to 0.12
	// - SinusL3: -0.12 to 0.12
	// - SinusL4: -0.12 to 0.12
	FSDI_FLOAT amplitude;

	// For future use
	FSDI_UINT8 reserved[8];
} FSDI_SFX_Effect;

/*
 * This structure defines special effects that are generated by the hardware and are independed to
 * the primary telemetry or positioning data stream. Not all devices support this feature.
 */
typedef struct FSDI_PACKED FSDI_SFX
{
	// Put here sizeof(FSDI_SFX).
	// NOTE: This field is mandatory.
	FSDI_UINT8  structSize;

	// Number of the effects specified in the below filed. From 0 to FSDI_SFX_MaxEffectsCount.
	FSDI_UINT8  effectsCount;

	// Specification of the effects
	FSDI_SFX_Effect effects[FSDI_SFX_MaxEffectsCount];
} FSDI_SFX;

#pragma pack(pop)

typedef enum FSDI_MaskBits
{
	FSDI_BIT_PAUSE          = (1 << 0),
	FSDI_BIT_POSITION       = (1 << 1),
	FSDI_BIT_TRANSFORMATION = (1 << 1),
	FSDI_BIT_MAX_SPEED      = (1 << 2),
	FSDI_BIT_STRATEGY       = (1 << 3),
} FSDI_MaskBits;

#endif
