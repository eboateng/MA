function out = FSMI_PlatformInfo()

	info.structSize                   = 174;
	info.timemark                     = 0;
	info.isConnected                  = false;
	info.state                        = 0;
	info.actualMotorPosition          = [ 0, 0, 0, 0, 0, 0 ];
	info.actualMotorSpeed             = [ 0, 0, 0, 0, 0, 0 ];
	info.isThermalProtectionActivated = false;
	info.worstModuleStatus            = 0;
	info.worstModuleStatusIndex       = 0;
	info.coolingSystemMalfunction     = false;
	info.isKinematicsSupported        = false;
	info.ikPrecision                  = [ 0, 0, 0, 0, 0, 0 ];
	info.ikRecentStatus               = false;
	info.fkRoll                       = 0;
	info.fkPitch                      = 0;
	info.fkYaw                        = 0;
	info.fkHeave                      = 0;
	info.fkSway                       = 0;
	info.fkSurge                      = 0;
	info.fkRecentStatus               = false;
	info.requiredMotorPosition        = [ 0, 0, 0, 0, 0, 0 ];
	info.fkRoll_deg                   = 0;
	info.fkPitch_deg                  = 0;
	info.fkYaw_deg                    = 0;
	info.fkRollSpeed_deg              = 0;
	info.fkPitchSpeed_deg             = 0;
	info.fkYawSpeed_deg               = 0;
	info.fkHeaveSpeed                 = 0;
	info.fkSwaySpeed                  = 0;
	info.fkSurgeSpeed                 = 0;
	info.fkRollAcc_deg                = 0;
	info.fkPitchAcc_deg               = 0;
	info.fkYawAcc_deg                 = 0;
	info.fkHeaveAcc                   = 0;
	info.fkSwayAcc                    = 0;
	info.fkSurgeAcc                   = 0;

	out = info;

end
