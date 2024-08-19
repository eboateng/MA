function FSMI_SendTopTablePosLog(api, position)
	position_C = libstruct('FSMI_TopTablePositionLogical', position);
	calllib(FSMI_Dll(),'ForceSeatMI_SendTopTablePosLog', api, position_C); 
	clear position_C;
end

