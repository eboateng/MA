api = FSMI_Create();

telemetry = FSMI_TelemetryACE();
iterator = 0;

FSMI_ActivateProfile(api, 'SDK - Vehicle Telemetry ACE');
FSMI_BeginMotionControl(api);

while iterator < 400
    telemetry.bodyLinearAcceleration.right = sin(iterator) * 10;
    
    FSMI_SendTelemetryACE(api, telemetry);
    
    iterator = iterator + 0.0005;

    info = FSMI_GetPlatformInfoEx(api);

    % Position in logical units
    fprintf('Position: [%i, %i, %i, %i, %i, %i]\n', info.actualMotorPosition);

    % Angles in [rad], travels in [mm]
    fprintf('    Actual Top Frame: roll %.3f, pitch %.3f, yaw %.3f, heave %.3f, surge %.3f, sway %.3f\n', info.fkRoll, info.fkPitch, info.fkYaw, info.fkHeave, info.fkSurge, info.fkSway);
end

FSMI_EndMotionControl(api);
FSMI_Delete(api);
