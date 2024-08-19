function out = FSMI_Telemetry()

	telemetry.structSize          = 177;
	telemetry.mask                = 524450;
	telemetry.state               = 0;
	telemetry.rpm                 = 0;
	telemetry.speed               = 0;
	telemetry.yaw                 = 0;
	telemetry.pitch               = 0;
	telemetry.roll                = 0;
	telemetry.yawAcceleration     = 0;
	telemetry.pitchAcceleration   = 0;
	telemetry.rollAcceleration    = 0;
	telemetry.yawSpeed            = 0;
	telemetry.pitchSpeed          = 0;
	telemetry.rollSpeed           = 0;
	telemetry.swayAcceleration    = 0;
	telemetry.heaveAcceleration   = 0;
	telemetry.surgeAcceleration   = 0;
	telemetry.swaySpeed           = 0;
	telemetry.heaveSpeed          = 0;
	telemetry.surgeSpeed          = 0;
	telemetry.gasPedalPosition    = 0;
	telemetry.brakePedalPosition  = 0;
	telemetry.clutchPedalPosition = 0;
	telemetry.gearNumber          = 0;
	telemetry.leftSideGroundType  = 0;
	telemetry.rightSideGroundType = 0;
	telemetry.collisionForce      = 0;
	telemetry.collisionYaw        = 0;
	telemetry.collisionPitch      = 0;
	telemetry.collisionRoll       = 0;
	telemetry.globalPositionX     = 0;
	telemetry.globalPositionY     = 0;
	telemetry.globalPositionZ     = 0;
	telemetry.timeMs              = 0;
	telemetry.triggers            = 0;
	telemetry.maxRpm              = 0;
	telemetry.flags               = 0;
	telemetry.aux                 = [ 0, 0, 0, 0, 0, 0, 0, 0 ];
	telemetry.extraYaw            = 0;
	telemetry.extraPitch          = 0;
	telemetry.extraRoll           = 0;
	telemetry.extraSway           = 0;
	telemetry.extraHeave          = 0;
	telemetry.extraSurge          = 0;

	out = telemetry;

end

