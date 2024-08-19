/*
 * Copyright (C) 2012-2022 MotionSystems
 * 
 * This file is part MSU SDK.
 *
 * www.motionsystems.eu
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */ 
#pragma once

#include <stdint.h>

#pragma pack(push, 1)
struct MSU_GyroPlayerData
{
	float accSway;
	float accSurge;
	float accHeave;
	float sway;
	float surge;
	float heave;
	float rollRate;
	float pitchRate;
	float yawRate;
	float roll;
	float pitch;
	float yaw;
	float accRoll;
	float accPitch;
	float accYaw;

	float extra[32];

	uint64_t timestamp_us;
};
#pragma pack(pop)

enum MSU_GyroPlayerErrorCode
{
	MSU_GPEC_Ok = 0,
	MSU_GPEC_EmptyFile,
	MSU_GPEC_ColumnIndexOutOfRange,
	MSU_GPEC_InvalidColumnMapping,
	MSU_GPEC_AlreadyPlaying,
	MSU_GPEC_InvalidPointer,
	MSU_GPEC_ParseError
};

enum MSU_GyroPlayerRole
{
	MSU_GPR_AccSway = 0, // m/s2
	MSU_GPR_AccSurge,
	MSU_GPR_AccHeave,

	MSU_GPR_Sway,      // m
	MSU_GPR_Surge,
	MSU_GPR_Heave,

	MSU_GPR_RollRate,  // rad/s
	MSU_GPR_PitchRate,
	MSU_GPR_YawRate,

	MSU_GPR_Roll, // rad
	MSU_GPR_Pitch,
	MSU_GPR_Yaw,

	//todo: make names more consistent
	MSU_GPR_AccRoll,
	MSU_GPR_AccPitch,
	MSU_GPR_AccYaw,

	MSU_GPR_Extra_0,
	MSU_GPR_Extra_1,
	MSU_GPR_Extra_2,
	MSU_GPR_Extra_3,
	MSU_GPR_Extra_4,
	MSU_GPR_Extra_5,
	MSU_GPR_Extra_6,
	MSU_GPR_Extra_7,
	MSU_GPR_Extra_8,
	MSU_GPR_Extra_9,
	MSU_GPR_Extra_10,
	MSU_GPR_Extra_11,
	MSU_GPR_Extra_12,
	MSU_GPR_Extra_13,
	MSU_GPR_Extra_14,
	MSU_GPR_Extra_15,
	MSU_GPR_Extra_16,
	MSU_GPR_Extra_17,
	MSU_GPR_Extra_18,
	MSU_GPR_Extra_19,
	MSU_GPR_Extra_20,
	MSU_GPR_Extra_21,
	MSU_GPR_Extra_22,
	MSU_GPR_Extra_23
};

enum MSU_GyroPlayerTimeUnit
{
	MSU_GPR_Milliseconds = 0,
	MSU_GPR_Seconds
};

enum MSU_GyroPlayerMappingInversion
{
	MSU_GPR_NotInverted = 0,
	MSU_GPR_Inverted
};

typedef void (*MSY_GyroPlayerCallback)(const MSU_GyroPlayerData* data, void* userData);
typedef void (*MSY_GyroPlayerFinishCallback)(void* userData);
