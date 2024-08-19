function out = FSMI_TelemetryACE()

	telemetry.structSize                     = 128;
	telemetry.state                          = 0;
	telemetry.gearNumber                     = 0;
	telemetry.accelerationPedalPosition      = 0;
	telemetry.brakePedalPosition             = 0;
	telemetry.clutchPedalPosition            = 0;
	telemetry.dummy1                         = 0;
	telemetry.dummy2                         = 0;
	telemetry.rpm                            = 0;
	telemetry.maxRpm                         = 0;
	telemetry.vehicleForwardSpeed            = 0;
	telemetry.bodyLinearAcceleration.right   = 0;
	telemetry.bodyLinearAcceleration.upward  = 0;
	telemetry.bodyLinearAcceleration.forward = 0;
	telemetry.bodyAngularVelocity.pitch      = 0;
	telemetry.bodyAngularVelocity.roll       = 0;
	telemetry.bodyAngularVelocity.yaw        = 0;
	telemetry.headPosition.right             = 0;
	telemetry.headPosition.upward            = 0;
	telemetry.headPosition.forward           = 0;
	telemetry.extraTranslation.right         = 0;
	telemetry.extraTranslation.upward        = 0;
	telemetry.extraTranslation.forward       = 0;
	telemetry.extraRotation.pitch            = 0;
	telemetry.extraRotation.roll             = 0;
	telemetry.extraRotation.yaw              = 0;
	telemetry.extraRotationCenter.right      = 0;
	telemetry.extraRotationCenter.upward     = 0;
	telemetry.extraRotationCenter.forward    = 0;
	telemetry.userAux                        = [ 0, 0, 0, 0, 0, 0, 0, 0 ];

	out = telemetry;

end
