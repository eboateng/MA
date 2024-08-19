function FSMI_SendTelemetryACE(api, telemetry)
	telemetry_C = libstruct('FSMI_TelemetryACE', telemetry);
	calllib(FSMI_Dll(),'ForceSeatMI_SendTelemetryACE', api, telemetry_C); 
	clear telemetry_C;
end

