#include "ForceSeatDI_Simulink.h"
#include "ForceSeatDI_Functions.h"

FSDI_Handle                         g_handle = 0;
FSDI_TopTablePositionLogical        g_positionLog;
FSDI_TopTablePositionPhysical       g_positionPhy;
FSDI_ActualActuatorsPositionLogical g_actualActuatorsPosition;
FSDI_ActualTopTablePositionPhysical g_topTablePosition;
FSDI_PlatformInfo                   g_platformInfo;

void initialize()
{
	g_handle = ForceSeatDI_Create();

	if (g_handle)
	{
		memset(&g_positionLog, 0, sizeof(g_positionLog));
		g_positionLog.structSize = sizeof(FSDI_TopTablePositionLogical);
		g_positionLog.mask       = FSDI_BIT_PAUSE | FSDI_BIT_POSITION | FSDI_BIT_MAX_SPEED;
		g_positionLog.maxSpeed   = 65535;
		g_positionLog.pause      = FSDI_False;

		memset(&g_positionPhy, 0, sizeof(g_positionPhy));
		g_positionPhy.structSize = sizeof(FSDI_TopTablePositionPhysical);
		g_positionPhy.mask       = FSDI_BIT_PAUSE | FSDI_BIT_POSITION | FSDI_BIT_MAX_SPEED | FSDI_BIT_STRATEGY;
		g_positionPhy.maxSpeed   = 65535;
		g_positionPhy.pause      = FSDI_False;
		g_positionPhy.strategy   = FSDI_Strategy_FullMatch;

		memset(&g_actualActuatorsPosition, 0, sizeof(g_actualActuatorsPosition));
		g_actualActuatorsPosition.structSize = sizeof(FSDI_ActualActuatorsPositionLogical);

		memset(&g_topTablePosition, 0, sizeof(g_topTablePosition));
		g_topTablePosition.structSize = sizeof(FSDI_ActualTopTablePositionPhysical);

		memset(&g_platformInfo, 0, sizeof(g_platformInfo));
		g_platformInfo.structSize = sizeof(FSDI_PlatformInfo);
	}
}

void terminate()
{
	if(0 != g_handle)
	{
		ForceSeatDI_Park(g_handle, FSDI_ParkMode_Normal);
		ForceSeatDI_Delete(g_handle);
		g_handle = 0;
	}
}

void Refresh(int enable)
{
	if (0 < enable)
	{
		ForceSeatDI_GetActuatorsPosLog(g_handle, &g_actualActuatorsPosition);
		ForceSeatDI_GetPlatformInfo(g_handle,    &g_platformInfo);
		ForceSeatDI_GetTopTablePosPhy(g_handle,  &g_topTablePosition);
	}
}

void SetPosPhy(double yaw_rad, double pitch_rad, double roll_rad, double heave_mm, double sway_mm, double surge_mm, int enable)
{
	if (0 >= enable)
	{
		return;
	}

	g_positionPhy.roll  = roll_rad;
	g_positionPhy.pitch = pitch_rad;
	g_positionPhy.yaw   = yaw_rad;
	g_positionPhy.heave = heave_mm;
	g_positionPhy.sway  = sway_mm; 
	g_positionPhy.surge = surge_mm;

	ForceSeatDI_SendTopTablePosPhy(g_handle, &g_positionPhy);

	Refresh(enable);
}

void SetPosLog(double yaw, double pitch, double roll, double heave, double sway, double surge, int enable)
{
	if (0 >= enable)
	{
		return;
	}

	g_positionLog.yaw   = yaw;
	g_positionLog.pitch = pitch;
	g_positionLog.roll  = roll;
	g_positionLog.heave = heave;
	g_positionLog.sway  = sway;
	g_positionLog.surge = surge;

	ForceSeatDI_SendTopTablePosLog(g_handle, &g_positionLog);

	Refresh(enable);
}

double GetYaw_rad()
{
	return g_topTablePosition.yaw;
}

double GetPitch_rad()
{
	return g_topTablePosition.pitch;
}

double GetRoll_rad()
{
	return g_topTablePosition.roll;
}

double GetHeave_mm()
{
	return g_topTablePosition.heave;
}

double GetSway_mm()
{
	return g_topTablePosition.sway;
}

double GetSurge_mm()
{
	return g_topTablePosition.surge;
}
