function out = FSMI_TopTablePositionPhysical()

	position.structSize = 65;
	position.mask       = 14;
	position.state      = 0;
	position.roll       = 0;
	position.pitch      = 0;
	position.yaw        = 0;
	position.heave      = 0;
	position.sway       = 0;
	position.surge      = 0;
	position.maxSpeed   = 65535;
	position.triggers   = 0;
	position.aux        = [ 0, 0, 0, 0, 0, 0, 0, 0 ];

	out = position;

end
