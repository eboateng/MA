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
#ifndef FORCE_SEAT_MI_POSITIONING_H
#define FORCE_SEAT_MI_POSITIONING_H

#include "ForceSeatMI_Common.h"

#pragma pack(push, 1)

/*
 * This structure defines position of top frame (table) in physical units (rad, mm).
 * It uses Inverse Kinematics module and it might not be supported by all motion platforms.
 * By default BestMatch strategy is used.
 */
typedef struct FSMI_TopTablePositionPhysical
{
	// Put here sizeof(FSMI_TopTablePositionPhysical).
	// NOTE: This field is mandatory.
	FSMI_UINT8  structSize; 

	// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
	FSMI_UINT32 mask;

	// State flag (bit fields, it is used to PAUSE or UNPAUSE the system).
	// NOTE: If the field is provided, set FSMI_POS_BIT_STATE in the mask.
	FSMI_UINT8  state;

	// NOTE: If below fields are provided, set FSMI_POS_BIT_POSITION in the mask.
	FSMI_FLOAT  roll;       // in radians, roll  < 0 = left,  roll > 0  = right
	FSMI_FLOAT  pitch;      // in radians, pitch < 0 = front, pitch > 0 = rear
	FSMI_FLOAT  yaw;        // in radians, yaw   < 0 = right, yaw > 0   = left
	FSMI_FLOAT  heave;      // in mm, heave < 0 - down, heave > 0 - top
	FSMI_FLOAT  sway;       // in mm, sway  < 0 - left, sway  > 0 - right
	FSMI_FLOAT  surge;      // in mm, surge < 0 - rear, surge > 0 - front

	// 0 to 65535, actual speed is not always equal to max speed due to ramps.
	// NOTE: If below fields are provided, set FSMI_POS_BIT_MAX_SPEED in the mask.
	FSMI_UINT16 maxSpeed;

	// State of buttons, gun triggers, etc. It is passed directly to the motion platform.
	// NOTE: If below fields are provided, set FSMI_POS_BIT_TRIGGERS in the mask.
	FSMI_UINT8  triggers;

	// Custom use field that can be used in script in ForceSeatPM.
	// NOTE: If below fields are provided, set FSMI_POS_BIT_AUX in the mask.
	FSMI_FLOAT  aux[8];

} FSMI_PACKED FSMI_TopTablePositionPhysical;

/*
 * This structure defines position of top frame (table) in physical units (rad, mm) by specifing transformation matrix.
 * It uses Inverse Kinematics module and it is dedicated for 6DoF motion platforms.
 * If matrix transformation is specified, the Inverse Kinematics module always uses FullMatch strategy.
 */
typedef struct FSMI_TopTableMatrixPhysical
{
	// Put here sizeof(FSMI_TopTableMatrixPhysical).
	// NOTE: This field is mandatory.
	FSMI_UINT8  structSize; 

	// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
	FSMI_UINT32 mask;

	// State flag (bit fields, it is used to PAUSE or UNPAUSE the system).
	// NOTE: If the field is provided, set FSMI_POS_BIT_STATE in the mask.
	FSMI_UINT8  state;

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
	// NOTE: If below fields are provided, set FSMI_POS_BIT_MATRIX in the mask.
	FSMI_FLOAT  transformation[4][4]; 

	// 0 to 65535, actual speed is not always equal to max speed due to ramps.
	// NOTE: If below fields are provided, set FSMI_POS_BIT_MAX_SPEED in the mask.
	FSMI_UINT16 maxSpeed;

	// State of buttons, gun triggers, etc. It is passed directly to the motion platform.
	// NOTE: If below fields are provided, set FSMI_POS_BIT_TRIGGERS in the mask.
	FSMI_UINT8  triggers;

	// Custom use field that can be used in script in ForceSeatPM.
	// NOTE: If below fields are provided, set FSMI_POS_BIT_AUX in the mask.
	FSMI_FLOAT  aux[8];

} FSMI_PACKED FSMI_TopTableMatrixPhysical;

/*
 * This structure defines position of top frame (table) in logical units (percents).
 * It does not use Inverse Kinematics module so rotation and movements are not always linear.
 *
 * DEPRECATED: It is recommended to use FSMI_TopTablePositionPhysical.
 */
typedef struct FSMI_TopTablePositionLogical
{
	// Put here sizeof(FSMI_TopTablePositionLogical).
	// NOTE: This field is mandatory.
	FSMI_UINT8  structSize; 

	// Set bits in the mask to tell the motion software which fields in this stucture are provided by your application.
	FSMI_UINT32 mask;

	// State flag (bit fields, it is used to PAUSE or UNPAUSE the system).
	// NOTE: If the field is provided, set FSMI_POS_BIT_STATE in the mask.
	FSMI_UINT8  state;

	// NOTE: If below fields are provided, set FSMI_POS_BIT_POSITION in the mask.
	FSMI_INT16  roll;       // -32767 max left,   +32767 max right
	FSMI_INT16  pitch;      // -32767 max rear,   +32767 max front
	FSMI_INT16  yaw;        // -32767 max left,   +32767 max right
	FSMI_INT16  heave;      // -32767 max bottom, +32767 max top
	FSMI_INT16  sway;       // -32767 max left,   +32767 max right
	FSMI_INT16  surge;      // -32767 max rear,   +32767 max front

	// 0 to 65535, actual speed is not always equal to max speed due to ramps.
	// NOTE: If below fields are provided, set FSMI_POS_BIT_MAX_SPEED in the mask.
	FSMI_UINT16 maxSpeed; 

	// State of buttons, gun triggers, etc. It is passed directly to the motion platform.
	// NOTE: If below fields are provided, set FSMI_POS_BIT_TRIGGERS in the mask.
	FSMI_UINT8  triggers; 

	// Custom use field that can be used in script in ForceSeatPM.
	// NOTE: If below fields are provided, set FSMI_POS_BIT_AUX in the mask.
	FSMI_FLOAT  aux[8];   

} FSMI_PACKED FSMI_TopTablePositionLogical;

#pragma pack(pop)

typedef enum FSMI_PositionMaskBits
{
	FSMI_POS_BIT_STATE     = (1 << 1),
	FSMI_POS_BIT_POSITION  = (1 << 2),
	FSMI_POS_BIT_MATRIX    = (1 << 2),
	FSMI_POS_BIT_MAX_SPEED = (1 << 3),
	FSMI_POS_BIT_TRIGGERS  = (1 << 4),
	FSMI_POS_BIT_AUX       = (1 << 5),
} FSMI_PositionMaskBits;

#endif
