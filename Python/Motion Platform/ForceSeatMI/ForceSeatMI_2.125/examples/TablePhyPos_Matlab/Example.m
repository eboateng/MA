api = FSMI_Create();

position = FSMI_TopTablePositionPhysical();
maxRoll  = 10;
iterator = 0;

FSMI_ActivateProfile(api, 'SDK - Positioning');
FSMI_BeginMotionControl(api);

while iterator < 50
    position.roll = sin(iterator) * maxRoll;
    
    FSMI_SendTopTablePosPhy(api, position);
    
    iterator = iterator + 0.001;

    info = FSMI_GetPlatformInfoEx(api);

    % Position in logical units
    fprintf('Position: [%i, %i, %i, %i, %i, %i]\n', info.actualMotorPosition);

    % Angles in [rad], travels in [mm]
    fprintf('    Actual Top Frame: roll %.3f, pitch %.3f, yaw %.3f, heave %.3f, surge %.3f, sway %.3f\n', info.fkRoll, info.fkPitch, info.fkYaw, info.fkHeave, info.fkSurge, info.fkSway);
end

FSMI_EndMotionControl(api);
FSMI_Delete(api);
