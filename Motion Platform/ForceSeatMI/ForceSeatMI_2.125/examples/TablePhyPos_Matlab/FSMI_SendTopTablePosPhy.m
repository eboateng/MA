function FSMI_SendTopTablePosPhy(api, position)
	position_C = libstruct('FSMI_TopTablePositionPhysical', position);
	calllib(FSMI_Dll(),'ForceSeatMI_SendTopTablePosPhy', api, position_C);
	clear position_C;
end

