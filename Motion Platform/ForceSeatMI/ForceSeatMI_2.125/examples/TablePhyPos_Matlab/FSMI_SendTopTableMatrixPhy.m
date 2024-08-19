function ForceSeatMI_SendTopTableMatrixPhy(api, position)
	position_C = libstruct('FSMI_TopTableMatrixPhysical', position);
	calllib(FSMI_Dll(),'ForceSeatMI_SendTopTableMatrixPhy', api, position_C);
	clear position_C;
end
