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
#ifndef FORCE_SEAT_MI_COMMON_H
#define FORCE_SEAT_MI_COMMON_H

#include "ForceSeatMI_Defines.h"

typedef unsigned char      FSMI_UINT8;
typedef signed   char      FSMI_INT8;
typedef unsigned int       FSMI_UINT32;
typedef signed   int       FSMI_INT32;
typedef unsigned short     FSMI_UINT16;
typedef signed   short     FSMI_INT16;
typedef float              FSMI_FLOAT;
typedef unsigned long long FSMI_UINT64;
typedef char               FSMI_Bool;
typedef double             FSMI_FLOAT64;

enum FSMI_Defines
{
	FSMI_True                = 1,
	FSMI_False               = 0,
	FSMI_MotorsCount         = 6,
	FSMI_SFX_MaxEffectsCount = 4,
	FSMI_UserAuxCount        = 8
};

enum FSMI_ParkMode
{
	FSMI_ParkMode_ToCenter     = 0,
	FSMI_ParkMode_Normal       = 1,
	FSMI_ParkMode_ForTransport = 2
};

// State flags
#define FSMI_STATE_NO_PAUSE    (0 << 0)
#define FSMI_STATE_PAUSE       (1 << 0)

#endif
