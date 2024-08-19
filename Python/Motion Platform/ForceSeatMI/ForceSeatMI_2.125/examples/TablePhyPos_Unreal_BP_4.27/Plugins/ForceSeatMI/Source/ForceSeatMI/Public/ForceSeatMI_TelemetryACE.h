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
#ifndef FORCE_SEAT_MI_TELEMETRY_ACE_H
#define FORCE_SEAT_MI_TELEMETRY_ACE_H

#include "ForceSeatMI_Common.h"

#pragma pack(push, 1)

typedef struct FSMI_TelemetryRUF
{
	FSMI_FLOAT right;   // + is right,   - is left
	FSMI_FLOAT upward;  // + is upward,  - is downward
	FSMI_FLOAT forward; // + is forward, - is backward
} FSMI_PACKED FSMI_TelemetryRUF;

typedef struct FSMI_TelemetryPRY
{
	FSMI_FLOAT pitch; // + front goes up,        - front goes down
	FSMI_FLOAT roll;  // + right side goes down, - right sidegoes up
	FSMI_FLOAT yaw;   // + front goes left,      - front goes right
} FSMI_PACKED FSMI_TelemetryPRY;

/*
 * This structure is used to forward vehicle physics data from application/game to the ForceSeatPM
 * where the data is next processed and transformed into motion platform top table's movements.
 */
typedef struct FSMI_TelemetryACE
{
	// Put here sizeof(FSMI_TelemetryACE).
	// MANDATORY: Yes
	FSMI_UINT8  structSize;

	// Only single bit is used in current version.
	//  (state & 0x01) == 0 -> no pause
	//  (state & 0x01) == 1 -> paused, FSMI_STATE_PAUSE
	FSMI_UINT8  state;

	FSMI_INT8  gearNumber;                // Current gear number, -1 for reverse, 0 for neutral, 1, 2, 3, ...
	FSMI_UINT8 accelerationPedalPosition; // 0 to 100 (in percent)
	FSMI_UINT8 brakePedalPosition;        // 0 to 100 (in percent)
	FSMI_UINT8 clutchPedalPosition;       // 0 to 100 (in percent)

	FSMI_UINT8 dummy1;
	FSMI_UINT8 dummy2;

	FSMI_UINT32 rpm;                 // Engine RPM
	FSMI_UINT32 maxRpm;              // Engine max RPM
	FSMI_FLOAT  vehicleForwardSpeed; // Forward speed, in [m/s]. For dashboard applications.

	// Lateral, vertical and longitudinal acceleration from the simulation.
	// UNITS: [m/s^2]
	// MANDATORY: Yes
	FSMI_TelemetryRUF bodyLinearAcceleration;

	// Angular rotation velocity about vertical, lateral and longitudinal axes.
	// UNITS: [radians/s]
	// MANDATORY: Yes
	FSMI_TelemetryPRY bodyAngularVelocity;

	// Position of the user head for accelerations and angular velocity effects generation
	// in relation to the top frame center.
	// UNITS: [meters]
	FSMI_TelemetryRUF headPosition;

	// Additional translation for the top frame.
	// UNITS: [meters]
	FSMI_TelemetryRUF extraTranslation;

	// Additional rotation for the top frame.
	// UNITS: [rad]
	FSMI_TelemetryPRY extraRotation;

	// Rotation center for the 'extra' transformation in relation to the top frame center.
	// UNITS: [meters]
	FSMI_TelemetryRUF extraRotationCenter;

	// Custom field that can be used in script in ForceSeatPM to trigger user defined actions.
	FSMI_FLOAT  userAux[FSMI_UserAuxCount];

	// Custom field that can be used in script in ForceSeatPM to trigger user defined actions.
	FSMI_UINT32 userFlags;

} FSMI_PACKED FSMI_TelemetryACE;
#pragma pack(pop)

#endif
