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

using System;
using System.Runtime.InteropServices;

namespace MotionSystems
{
	///
	/// List of possible module status values
	///
	public enum FSDI_ModuleStatus
	{
		Ok                    = 0,
		Overheated            = 1,
		Communication_Error   = 2,
		Config_Error          = 3,
		RefPointSwitch_Error  = 4,
		Calibration_Error     = 5,
		General_Error         = 6,
		NotConnected_Error    = 7,
		NoPowerSupply_Error   = 8,
		FanSpeedTooLow_Error  = 9,
		OutOfRange_Error      = 10,
		StallDetected_Error   = 11,
		EMCY_Error            = 12,
		Overcurrent_Error     = 13,
		Overloaded_Error      = 14
	};

	///
	/// Possible operating states of the motion platform.
	///
	public struct FSDI_State
	{
		public const int AnyPaused            = 1 << 0; // 0b00000001
		public const int ParkingCompleted     = 1 << 1; // 0b00000010
		public const int Offline              = 1 << 2; // 0b00000100

		// Only one park mode is valid at given time.
		// Greater parking mode value (number) = greater priority.
		// It means that transport parking wins over normal parking
		// and normal parking wins over parking to center.
		public const int ParkModeMask         = 0xE0;   // 0b11100000
		public const int NoParking            = 0x00;   // 0b00000000
		public const int SoftParkToCenter     = 0x20;   // 0b00100000
		public const int SoftParkNormal       = 0x40;   // 0b01000000
		public const int SoftParkForTransport = 0x60;   // 0b01100000
		public const int HardParkToCenter     = 0x80;   // 0b10000000
		public const int HardParkNormal       = 0xA0;   // 0b10100000
		public const int HardParkForTransport = 0xC0;   // 0b11000000
		public const int Reserved             = 0xE0;   // 0b11100000
	};

	/*
	 * Possible parking modes for the motion platform.
	 */
	public enum FSDI_ParkMode
	{
		Normal       = 0,
		ToCenter     = 1,
		ForTransport = 2
	};

	///
	/// Strategy for finding reachable position from request position. It is used by Inverse Kinematics module.
	///
	public enum FSDI_Strategy
	{
		FullMatch = 0, // Go to required position or report an error if the position is outside work envelop
		BestMatch = 1  // Go to closest reachable position
	};

	public enum FSDI_SFX_EffectType
	{
		// Effect is appended to the required position before trajectory generator uses the position.
		// It means that this effect affects state of the adaptive algorithms which are used to adjust motion platform
		// operation parameters to the signal dynamics. This effect might be also less pronounced if trajectory generator
		// is configured to use long acceleration ramps.
		// It is supported only by "PS" and "QS" motion platforms.
		SinusLevel2 = 0,

		// Effect is appended to the output (position) of the trajectory generator. It is NOT affected by the
		// trajectory generator configuration and it does NOT affect the adaptive algorithms. Result of this effect
		// might be different depending on the actuator hardware configuration (length/pitch).
		// It is supported only by "QS" motion platforms.
		SinusLevel3 = 1,

		// Effect is appended to the final motor torque. It is NOT affected by the trajectory generator and other
		// control subsystems. Result of this effect might be different depending on the actuator hardware
		// configuration (length/pitch).
		// It is supported only by "QS" motion platforms.
		SinusLevel4 = 2
	};

	public struct FSDI_SFX_AreaFlags
	{
		// Effect is generated on the actuator(s) assigned to the front-left area of the cockpit.
		public const int FrontLeft  = 1 << 0;

		// Effect is generated on the actuator(s) assigned to the front-right area of the cockpit.
		public const int FrontRight = 1 << 1;

		// Effect is generated on the actuator(s) assigned to the rear-left area of the cockpit.
		public const int RearLeft   = 1 << 2;

		// Effect is generated on the actuator(s) assigned to the rear-right area of the cockpit.
		public const int RearRight  = 1 << 3;

		// Effect is generated on auxiliary actuator, e.g. seat belt tensioner.
		public const int Aux1       = 1 << 4;

		// Effect is generated on auxiliary actuator, e.g. seat belt tensioner.
		public const int Aux2       = 1 << 5;
	};

	///
	/// This structure defines position of top frame (table) in logical units (percents).
	/// It does not use invese kinematics module so rotation and movements are not always linear.
	///
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct FSDI_TopTablePositionLogical
	{
		// Put here sizeof(FSDI_TopTablePositionLogical).
		// NOTE: This field is mandatory.
		public byte structSize;

		// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
		public uint mask;

		// NOTE: If the field is provided, set FSDI_BIT.PAUSE in the mask.
		public byte pause;

		// NOTE: If below fields are provided, set FSDI_BIT.POSITION in the mask.
		public short roll;       // -32k max left,   +32k max right
		public short pitch;      // -32k max rear,   +32k max front
		public short yaw;        // -32k max left,   +32k max right
		public short heave;      // -32k max bottom, +32k max top
		public short sway;       // -32k max left,   +32k max right
		public short surge;      // -32k max rear,   +32k max front

		// 0 to 64k, actual speed is not always equal to max speed due to ramps.
		// NOTE: If below fields are provided, set FSDI_BIT.MAX_SPEED in the mask.
		public ushort maxSpeed;
	}

	///
	/// This structure defines position of top frame (table) in physical units (rad, mm).
	/// It uses Inverse Kinematics module and it might not be supported by all motion platforms.
	/// By default FullMatch strategy is used.
	///
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct FSDI_TopTablePositionPhysical
	{
		// Put here sizeof(FSDI_TopTablePositionPhysical).
		// NOTE: This field is mandatory.
		public byte structSize;

		// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
		public uint mask;

		// NOTE: If the field is provided, set FSDI_BIT.PAUSE in the mask.
		public byte pause;

		// NOTE: If below fields are provided, set FSDI_BIT.POSITION in the mask.
		public float roll;       // in radians, roll  < 0 = left,  roll > 0  = right
		public float pitch;      // in radians, pitch < 0 = front, pitch > 0 = rear
		public float yaw;        // in radians, yaw   < 0 = right, yaw > 0   = left
		public float heave;      // in mm, heave < 0 - down, heave > 0 - top
		public float sway;       // in mm, sway  < 0 - left, sway  > 0 - right
		public float surge;      // in mm, surge < 0 - rear, surge > 0 - front

		// 0 to 64k, actual speed is not always equal to max speed due to ramps.
		// NOTE: If below fields are provided, set FSDI_BIT.MAX_SPEED in the mask.
		public ushort maxSpeed;

		// One of FSDI_Strategy.
		// NOTE: If below fields are provided, set FSDI_BIT.STRATEGY in the mask.
		public byte   strategy;  
	}

	///
	/// This structure defines position of top frame (table) in physical units (rad, mm) by specifing transformation matrix.
	/// It uses Inverse Kinematics module and it is dedicated for 6DoF motion platforms.
	/// If matrix transformation is specified, the Inverse Kinematics module always uses FullMatch strategy.
	///
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct FSDI_TopTableMatrixPhysical
	{
		// Put here sizeof(FSDI_TopTableMatrixPhysical).
		// NOTE: This field is mandatory.
		public byte structSize;

		// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
		public uint mask;

		// NOTE: If the field is provided, set FSDI_BIT.PAUSE in the mask.
		public byte pause;

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
		// NOTE: If below fields are provided, set FSDI_BIT.TRANSFORMATION in the mask.
		public float m11;        // 3D transformation matrix, 1st row
		public float m12;        // 3D transformation matrix, 1st row
		public float m13;        // 3D transformation matrix, 1st row
		public float m14;        // 3D transformation matrix, 1st row

		public float m21;        // 3D transformation matrix, 2nd row
		public float m22;        // 3D transformation matrix, 2nd row
		public float m23;        // 3D transformation matrix, 2nd row
		public float m24;        // 3D transformation matrix, 2nd row

		public float m31;        // 3D transformation matrix, 3rd row
		public float m32;        // 3D transformation matrix, 3rd row
		public float m33;        // 3D transformation matrix, 3rd row
		public float m34;        // 3D transformation matrix, 3rd row

		public float m41;        // 3D transformation matrix, 4rd row
		public float m42;        // 3D transformation matrix, 4rd row
		public float m43;        // 3D transformation matrix, 4rd row
		public float m44;        // 3D transformation matrix, 4rd row

		// 0 to 64k, actual speed is not always equal to max speed due to ramps.
		// NOTE: If below fields are provided, set FSDI_BIT.MAX_SPEED in the mask.
		public ushort maxSpeed;
	}

	///
	/// This structure defines position of actuators arms
	///
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct FSDI_RequiredActuatorsPositionLogical
	{
		// Put here sizeof(FSDI_RequiredActuatorsPositionLogical).
		// NOTE: This field is mandatory.
		public byte structSize;

		// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
		public uint mask;

		// NOTE: If the field is provided, set FSDI_BIT.PAUSE in the mask.
		public byte pause;

		// 0 min, +64k max
		// NOTE: If below fields are provided, set FSDI_BIT.POSITION in the mask.
		public ushort actuatorPosition1;
		public ushort actuatorPosition2;
		public ushort actuatorPosition3;
		public ushort actuatorPosition4;
		public ushort actuatorPosition5;
		public ushort actuatorPosition6;

		// 0 to 64k, actual speed is not always equal to max speed due to ramps.
		// NOTE: If below fields are provided, set FSDI_BIT.MAX_SPEED in the mask.
		public ushort maxSpeed;
	}

	///
	/// Actual platform status received from the controller
	///
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct FSDI_PlatformInfo
	{
		// Set this to sizeof(FSDI_PlatformInfo) before calling 'get' function.
		public byte structSize; 

		// Check FSDI_State for bit field explanation.
		public uint state;

		public byte isThermalProtectionActivated;     // global thermal protection status
		public byte coolingSystemMalfunction;         // global cooling system status
		public byte moduleStatus1;   // status of modules (actuator or CAN node) - one of FSDI_ModuleStatus
		public byte moduleStatus2;   // status of modules (actuator or CAN node) - one of FSDI_ModuleStatus
		public byte moduleStatus3;   // status of modules (actuator or CAN node) - one of FSDI_ModuleStatus
		public byte moduleStatus4;   // status of modules (actuator or CAN node) - one of FSDI_ModuleStatus
		public byte moduleStatus5;   // status of modules (actuator or CAN node) - one of FSDI_ModuleStatus
		public byte moduleStatus6;   // status of modules (actuator or CAN node) - one of FSDI_ModuleStatus
		public byte serialNumber1;
		public byte serialNumber2;
		public byte serialNumber3;
		public byte serialNumber4;
		public byte serialNumber5;
		public byte serialNumber6;
		public byte serialNumber7;
		public byte serialNumber8;
		public byte serialNumber9;
		public byte serialNumber10;
		public byte serialNumber11;
		public byte serialNumber12;
	}

	///
	/// Actual actuators position received from the controller
	///
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct FSDI_ActualActuatorsPositionLogical
	{
		// Set this to to sizeof(FSDI_ActualActuatorsPositionLogical) before calling 'get' function.
		public byte structSize; 

		// Check FSDI_State for bit field explanation.
		public byte state;

		public ushort actualMotorPosition1;
		public ushort actualMotorPosition2;
		public ushort actualMotorPosition3;
		public ushort actualMotorPosition4;
		public ushort actualMotorPosition5;
		public ushort actualMotorPosition6;
		public int    actualMotorSpeed1;
		public int    actualMotorSpeed2;
		public int    actualMotorSpeed3;
		public int    actualMotorSpeed4;
		public int    actualMotorSpeed5;
		public int    actualMotorSpeed6;

		public ushort requiredMotorPosition1;
		public ushort requiredMotorPosition2;
		public ushort requiredMotorPosition3;
		public ushort requiredMotorPosition4;
		public ushort requiredMotorPosition5;
		public ushort requiredMotorPosition6;
		public ushort maxAllowedMotorSpeed1_obsolete;
		public ushort maxAllowedMotorSpeed2_obsolete;
		public ushort maxAllowedMotorSpeed3_obsolete;
		public ushort maxAllowedMotorSpeed4_obsolete;
		public ushort maxAllowedMotorSpeed5_obsolete;
		public ushort maxAllowedMotorSpeed6_obsolete;
	}

	///
	/// This structure defines position of top frame (table) in physical units (rad, mm).
	/// It is calculated by forward kinematics from actual actuators position.
	///
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct FSDI_ActualTopTablePositionPhysical
	{
		// Set this to to sizeof(FSDI_ActualTopTablePositionPhysical) before calling 'get' function.
		public byte structSize;

		// Check FSDI_State for bit field explanation.
		public byte state;

		public float roll;   // in radians, roll  < 0 = left,  roll > 0  = right
		public float pitch;  // in radians, pitch < 0 = front, pitch > 0 = rear
		public float yaw;    // in radians, yaw   < 0 = right, yaw > 0   = left
		public float heave;  // in mm, heave < 0 - down, heave > 0 - top
		public float sway;   // in mm, sway  < 0 - left, sway  > 0 - right
		public float surge;  // in mm, surge < 0 - rear, surge > 0 - front
	};

	///
	/// This structure defines special effects that are generated by the hardware and are independed to
	/// the primary telemetry or positioning data stream. Not all devices support this feature.
	///
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct FSDI_SFX
	{
		// Put here sizeof(FSDI_SFX).
		// NOTE: This field is mandatory.
		public byte structSize;

		// Number of the effects specified in the below filed. From 0 to FSDI_SFX_MaxEffectsCount.
		public byte effectsCount;

		// Effect type. It is one of FSDI_SFX_EffectType.
		public byte effect1Type;

		// Area where the effect should be generated. It is combination of FSDI_SFX_AreaFlags.
		public ushort effect1Area;

		// Frequency (Hz) of the effect, from 0 to 255. Not all effects support all frequencies.
		// - SinusL2: 0 to 100
		// - SinusL3: 0 to 100
		// - SinusL4: 0 to 100
		public byte effect1Frequency;

		// Amplitude of the effect, from -1 to 1. Not all effects support full amplitudes.
		// - SinusL2: -0.12 to 0.12
		// - SinusL3: -0.12 to 0.12
		// - SinusL4: -0.12 to 0.12
		public float effect1Amplitude;

		// For future use
		public byte effect1Reserved1;
		public byte effect1Reserved2;
		public byte effect1Reserved3;
		public byte effect1Reserved4;
		public byte effect1Reserved5;
		public byte effect1Reserved6;
		public byte effect1Reserved7;
		public byte effect1Reserved8;

		// Effect type. It is one of FSDI_SFX_EffectType.
		public byte effect2Type;

		// Area where the effect should be generated. It is combination of FSDI_SFX_AreaFlags.
		public ushort effect2Area;

		// Frequency (Hz) of the effect, from 0 to 255. Not all effects support all frequencies.
		// - SinusL2: 0 to 100
		// - SinusL3: 0 to 100
		// - SinusL4: 0 to 100
		public byte effect2Frequency;

		// Amplitude of the effect, from -1 to 1. Not all effects support full amplitudes.
		// - SinusL2: -0.12 to 0.12
		// - SinusL3: -0.12 to 0.12
		// - SinusL4: -0.12 to 0.12
		public float effect2Amplitude;

		// For future use
		public byte effect2Reserved1;
		public byte effect2Reserved2;
		public byte effect2Reserved3;
		public byte effect2Reserved4;
		public byte effect2Reserved5;
		public byte effect2Reserved6;
		public byte effect2Reserved7;
		public byte effect2Reserved8;

		// Effect type. It is one of FSDI_SFX_EffectType.
		public byte effect3Type;

		// Area where the effect should be generated. It is combination of FSDI_SFX_AreaFlags.
		public ushort effect3Area;

		// Frequency (Hz) of the effect, from 0 to 255. Not all effects support all frequencies.
		// - SinusL2: 0 to 100
		// - SinusL3: 0 to 100
		// - SinusL4: 0 to 100
		public byte effect3Frequency;

		// Amplitude of the effect, from -1 to 1. Not all effects support full amplitudes.
		// - SinusL2: -0.12 to 0.12
		// - SinusL3: -0.12 to 0.12
		// - SinusL4: -0.12 to 0.12
		public float effect3Amplitude;

		// For future use
		public byte effect3Reserved1;
		public byte effect3Reserved2;
		public byte effect3Reserved3;
		public byte effect3Reserved4;
		public byte effect3Reserved5;
		public byte effect3Reserved6;
		public byte effect3Reserved7;
		public byte effect3Reserved8;
		
		// Effect type. It is one of FSDI_SFX_EffectType.
		public byte effect4Type;

		// Area where the effect should be generated. It is combination of FSDI_SFX_AreaFlags.
		public ushort effect4Area;

		// Frequency (Hz) of the effect, from 0 to 255. Not all effects support all frequencies.
		// - SinusL2: 0 to 100
		// - SinusL3: 0 to 100
		// - SinusL4: 0 to 100
		public byte effect4Frequency;

		// Amplitude of the effect, from -1 to 1. Not all effects support full amplitudes.
		// - SinusL2: -0.12 to 0.12
		// - SinusL3: -0.12 to 0.12
		// - SinusL4: -0.12 to 0.12
		public float effect4Amplitude;

		// For future use
		public byte effect4Reserved1;
		public byte effect4Reserved2;
		public byte effect4Reserved3;
		public byte effect4Reserved4;
		public byte effect4Reserved5;
		public byte effect4Reserved6;
		public byte effect4Reserved7;
		public byte effect4Reserved8;
	}

	///
	/// Helpers for mask bits
	///
	public struct FSDI_BIT
	{
		public const int PAUSE     = (1 << 0);
		public const int POSITION  = (1 << 1);
		public const int MAX_SPEED = (1 << 2);
		public const int STRATEGY  = (1 << 3);
	}
}
