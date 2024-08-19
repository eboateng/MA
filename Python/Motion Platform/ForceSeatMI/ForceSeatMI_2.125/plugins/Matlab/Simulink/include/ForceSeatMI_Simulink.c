#include "ForceSeatMI_Simulink.h"
#include "ForceSeatMI_Functions.h"

FSMI_Handle                   g_handle = 0;
FSMI_TopTablePositionPhysical g_positionPhy;
FSMI_TopTablePositionLogical  g_positionLog;
FSMI_PlatformInfo             g_platformInfo;

void initialize()
{
	g_handle = ForceSeatMI_Create();

	if (g_handle)
	{
		memset(&g_positionPhy, 0, sizeof(g_positionPhy));
		g_positionPhy.structSize = sizeof(g_positionPhy);
		g_positionPhy.mask = FSMI_POS_BIT_STATE | FSMI_POS_BIT_POSITION | FSMI_POS_BIT_MAX_SPEED;

		memset(&g_positionLog, 0, sizeof(g_positionLog));
		g_positionLog.structSize = sizeof(g_positionLog);
		g_positionLog.mask = FSMI_POS_BIT_STATE | FSMI_POS_BIT_POSITION | FSMI_POS_BIT_MAX_SPEED;

		memset(&g_platformInfo, 0, sizeof(g_platformInfo));

		ForceSeatMI_BeginMotionControl(g_handle);
		ForceSeatMI_ActivateProfile(g_handle, "SDK - Positioning");
	}
}

void terminate()
{
	if(0 != g_handle)
	{
		ForceSeatMI_EndMotionControl(g_handle);
		ForceSeatMI_Delete(g_handle);
		g_handle = 0;
	}
}

void Refresh(int enable)
{
	if (0 < enable)
	{
		ForceSeatMI_GetPlatformInfoEx(g_handle, &g_platformInfo, sizeof(g_platformInfo), 100);
	}
}

void SetPosPhy(double yaw_rad, double pitch_rad, double roll_rad, double heave_mm, double sway_mm, double surge_mm, int enable)
{
	if (0 >= enable)
	{
		return;
	}

	g_positionPhy.state    = FSMI_STATE_NO_PAUSE;
	g_positionPhy.maxSpeed = 65535;
	g_positionPhy.yaw      = yaw_rad;
	g_positionPhy.pitch    = pitch_rad;
	g_positionPhy.roll     = roll_rad;
	g_positionPhy.heave    = heave_mm;
	g_positionPhy.sway     = sway_mm;
	g_positionPhy.surge    = surge_mm;

	ForceSeatMI_SendTopTablePosPhy(g_handle, &g_positionPhy);
	ForceSeatMI_GetPlatformInfoEx(g_handle, &g_platformInfo, sizeof(g_platformInfo), 100);
}

void SetPosLog(double yaw, double pitch, double roll, double heave, double sway, double surge, int enable)
{
	if (0 >= enable)
	{
		return;
	}

	g_positionLog.state    = FSMI_STATE_NO_PAUSE;
	g_positionLog.maxSpeed = 65535;
	g_positionLog.yaw      = yaw;
	g_positionLog.pitch    = pitch;
	g_positionLog.roll     = roll;
	g_positionLog.heave    = heave;
	g_positionLog.sway     = sway;
	g_positionLog.surge    = surge;

	ForceSeatMI_SendTopTablePosLog(g_handle, &g_positionLog);
	ForceSeatMI_GetPlatformInfoEx(g_handle, &g_platformInfo, sizeof(g_platformInfo), 100);
}

double GetYaw_rad()
{
	return g_platformInfo.fkYaw;
}

double GetPitch_rad()
{
	return g_platformInfo.fkPitch;
}

double GetRoll_rad()
{
	return g_platformInfo.fkRoll;
}

double GetHeave_mm()
{
	return g_platformInfo.fkHeave;
}

double GetSway_mm()
{
	return g_platformInfo.fkSway;
}

double GetSurge_mm()
{
	return g_platformInfo.fkSurge;
}
