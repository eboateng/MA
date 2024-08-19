function FSMI_SendTelemetry(api, telemetry)
	telemetry_C = libstruct('FSMI_Telemetry', telemetry);
	calllib(FSMI_Dll(),'ForceSeatMI_SendTelemetry', api, telemetry_C); 
	clear telemetry_C;
end

