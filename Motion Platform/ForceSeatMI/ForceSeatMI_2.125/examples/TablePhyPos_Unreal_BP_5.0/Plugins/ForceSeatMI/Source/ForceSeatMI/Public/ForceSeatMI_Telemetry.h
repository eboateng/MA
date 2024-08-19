/*
 * Copyright (C) 2012-2022 MotionSystems
 * 
 * This file is part of ForceSeatMI SDK.
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
#ifndef FORCE_SEAT_MI_TELEMETRY_H
#define FORCE_SEAT_MI_TELEMETRY_H

#include "ForceSeatMI_Common.h"

#pragma pack(push, 1)

/*
 * This interface can used to get motion data from application or game.
 *
 * When the ForceSeatMI telemetry is implemented in application, any motion software is able to gather simulation data and
 * control motion platform (2DOF, 3DOF or 6DOF). This structure should be filled by the application and sent to motion processing 
 * system. It is require to fill "structSize" and "mask", other fields are OPTIONAL. It means that the application does not
 * have to support or provide all parameters mentioned below, but it is good to provide as much as possible.
 *
 * DEPRECATED: For new applications use FSMI_TelemetryACE.
 *
 * FIELDS LEGEND:
 * ==============
 * All values are in local vehicle coordinates.
 *
 *   YAW   - rotation along vertical axis,
 *          > 0 when vehicle front is rotating right,
 *          < 0 when vehicle front is rotating left
 *   PITCH - rotation along lateral axis,
 *          > 0 when vehicle front is rotating up
 *          > 0 when vehicle front is rotating down
 *   ROLL  - rotation along longitudinal axis,
 *          > 0 when vehicle highest point is rotating right,
 *          < 0 when vehicle highest point is rotating left
 *   SWAY  - transition along lateral axis,
 *          > 0 when vehicle is moving right,
 *          < 0 when vehicle is moving left
 *   HEAVE - transition along vertical axis,
 *          > 0 when vehicle is moving up,
 *          < 0 when vehicle is moving down
 *   SURGE - transition along longitudinal axis,
 *          > 0 when vehicle is moving forward,
 *          < 0 when vehicle is moving backward
 *
 * Please check below link for details description of yaw, pitch and roll:
 * http://en.wikipedia.org/wiki/Ship_motions
 */
typedef struct FSMI_Telemetry
{
	// Put here sizeof(FSMI_Telemetry).
	// NOTE: This field is mandatory.
	FSMI_UINT8  structSize;

	// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
	FSMI_UINT32 mask;

	// Only single bit is used in current version.
	//  (state & 0x01) == 0 -> no pause
	//  (state & 0x01) == 1 -> paused
	// NOTE: If the field is provided, set FSMI_TEL_BIT_STATE in the mask.
	FSMI_UINT8  state;

	// Engine RPM.
	// NOTE: If the field is provided, set FSMI_TEL_BIT_RPM in the mask.
	FSMI_UINT32 rpm;

	// Vehicle speed in m/s, can be < 0 for reverse.
	// NOTE: If this field is provided, set FSMI_TEL_BIT_SPEED in the mask.
	FSMI_FLOAT  speed;

	// NOTE: If below fields are provided, set FSMI_TEL_BIT_YAW_PITCH_ROLL in the mask.
	FSMI_FLOAT yaw;   // yaw in radians
	FSMI_FLOAT pitch; // vehicle pitch in radians
	FSMI_FLOAT roll;  // vehicle roll in radians

	// NOTE: If below fields are provided, set FSMI_TEL_BIT_YAW_PITCH_ROLL_ACCELERATION in the mask.
	FSMI_FLOAT yawAcceleration;   // radians/s^2
	FSMI_FLOAT pitchAcceleration; // radians/s^2
	FSMI_FLOAT rollAcceleration;  // radians/s^2

	// NOTE: If below fields are provided, set FSMI_TEL_BIT_YAW_PITCH_ROLL_SPEED in the mask.
	FSMI_FLOAT yawSpeed;   // radians/s
	FSMI_FLOAT pitchSpeed; // radians/s
	FSMI_FLOAT rollSpeed;  // radians/s

	// NOTE: If below fields are provided, set FSMI_TEL_BIT_SWAY_HEAVE_SURGE_ACCELERATION in the mask.
	FSMI_FLOAT swayAcceleration;  // m/s^2
	FSMI_FLOAT heaveAcceleration; // m/s^2
	FSMI_FLOAT surgeAcceleration; // m/s^2

	// NOTE: If below fields are provided, set FSMI_TEL_BIT_SWAY_HEAVE_SURGE_SPEED in the mask.
	FSMI_FLOAT swaySpeed;  // m/s
	FSMI_FLOAT heaveSpeed; // m/s
	FSMI_FLOAT surgeSpeed; // m/s

	// NOTE: If below fields are provided, set FSMI_TEL_BIT_PEDALS_POSITION in the mask.
	FSMI_UINT8 gasPedalPosition;    // 0 to 100 (in percent)
	FSMI_UINT8 brakePedalPosition;  // 0 to 100 (in percent)
	FSMI_UINT8 clutchPedalPosition; // 0 to 100 (in percent)

	// Current gear number, -1 for reverse, 0 for neutral, 1, 2, 3, ...
	// NOTE: If the field is provided, set FSMI_TEL_BIT_GEAR_NUMBER in the mask.
	FSMI_INT8  gearNumber;

	// Ground type, e.g. grass, dirt, gravel, please check FSMI_GroundType for details.
	// NOTE: If below fields are provided, set FSMI_TEL_BIT_GROUND_TYPE in the mask.
	FSMI_UINT8 leftSideGroundType;
	FSMI_UINT8 rightSideGroundType;

	// NOTE: If below fields are provided, set FSMI_TEL_BIT_COLLISION in the mask.
	FSMI_FLOAT collisionForce; // in Newtons (N), zero when there is no collision
	FSMI_FLOAT collisionYaw;   // yaw, pitch and roll for start point of collision force vector, end point is always in vehicle center
	FSMI_FLOAT collisionPitch;
	FSMI_FLOAT collisionRoll;

	// Global position,.
	// NOTE: If below fields are provided, set FSMI_TEL_BIT_GLOBAL_POSITION in the mask.
	FSMI_FLOAT globalPositionX; // right
	FSMI_FLOAT globalPositionY; // up
	FSMI_FLOAT globalPositionZ; // forward

	// Simulation time in milliseconds, can be relative (e.g. 0 means means application has just started).
	// NOTE: If the field is provided, set FSMI_TEL_BIT_TIME in the mask.
	FSMI_UINT32 timeMs;

	// State of buttons, gun triggers, etc. It is passed directly to the motion platform.
	// NOTE: If the field is provided, set FSMI_TEL_BIT_TRIGGERS in the mask.
	FSMI_UINT8  triggers;

	// Engine maximum RPM, it is used to simulate rev limiter.
	// NOTE: If the field is provided, set FSMI_TEL_BIT_MAX_RPM in the mask.
	FSMI_UINT32 maxRpm;

	// Combination of FSMI_Flags.
	// NOTE: If the field is provided, set FSMI_TEL_BIT_FLAGS in the mask.
	FSMI_UINT32 flags;

	// Custom field that can be used in script in ForceSeatPM to trigger user defined actions.
	// NOTE: If below fields are provided, set FSMI_TEL_BIT_AUX in the mask.
	FSMI_FLOAT aux[8];

	// Extra values that will be added at the end of signal processing. Those values will NOT be
	// filtered, smoothed or processed in any way. They might be used to generate additional
	// vibrations or custom effects.
	// NOTE: If felds below are provided, set FSMI_TEL_BIT_EXTRA_YAW_PITCH_ROLL_SWAY_HEAVE_SURGE in the mask.
	FSMI_FLOAT extraYaw;   // rad
	FSMI_FLOAT extraPitch; // rad
	FSMI_FLOAT extraRoll;  // rad
	FSMI_FLOAT extraSway;  // mm
	FSMI_FLOAT extraHeave; // mm
	FSMI_FLOAT extraSurge; // mm

} FSMI_PACKED FSMI_Telemetry;
#pragma pack(pop)

typedef enum FSMI_GroundType
{
	FSMI_UknownGround      = 0,
	FSMI_TarmacGround      = 1,
	FSMI_GrassGround       = 2,
	FSMI_DirtGround        = 3,
	FSMI_GravelGround      = 4,
	FSMI_RumbleStripGround = 5
} FSMI_GroundType;

typedef enum FSMI_Flags
{
	FSMI_ShiftLight      = (1 << 0),
	FSMI_AbsIsWorking    = (1 << 1),
	FSMI_EspIsWorking    = (1 << 2),
	FSMI_FrontWheelDrive = (1 << 3),
	FSMI_RearWheelDrive  = (1 << 4)
} FSMI_Flags;

typedef enum FSMI_TelemetryMaskBits
{
	FSMI_TEL_BIT_STATE                                 = (1 << 1),
	FSMI_TEL_BIT_RPM                                   = (1 << 2),
	FSMI_TEL_BIT_SPEED                                 = (1 << 3),

	FSMI_TEL_BIT_YAW_PITCH_ROLL                        = (1 << 4),
	FSMI_TEL_BIT_YAW_PITCH_ROLL_ACCELERATION           = (1 << 5),
	FSMI_TEL_BIT_YAW_PITCH_ROLL_SPEED                  = (1 << 6),
	FSMI_TEL_BIT_SWAY_HEAVE_SURGE_ACCELERATION         = (1 << 7),
	FSMI_TEL_BIT_SWAY_HEAVE_SURGE_SPEED                = (1 << 8),

	FSMI_TEL_BIT_PEDALS_POSITION                       = (1 << 9),
	FSMI_TEL_BIT_GEAR_NUMBER                           = (1 << 10),
	FSMI_TEL_BIT_GROUND_TYPE                           = (1 << 11),
	FSMI_TEL_BIT_COLLISION                             = (1 << 12),

	FSMI_TEL_BIT_GLOBAL_POSITION                       = (1 << 13),
	FSMI_TEL_BIT_TIME                                  = (1 << 14),
	FSMI_TEL_BIT_TRIGGERS                              = (1 << 15),

	FSMI_TEL_BIT_MAX_RPM                               = (1 << 16),
	FSMI_TEL_BIT_FLAGS                                 = (1 << 17),
	FSMI_TEL_BIT_AUX                                   = (1 << 18),
	FSMI_TEL_BIT_EXTRA_YAW_PITCH_ROLL_SWAY_HEAVE_SURGE = (1 << 19),
} FSMI_TelemetryMaskBits;

#endif
