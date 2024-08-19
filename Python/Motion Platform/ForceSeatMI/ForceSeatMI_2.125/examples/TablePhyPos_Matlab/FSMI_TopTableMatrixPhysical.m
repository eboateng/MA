function out = FSMI_TopTableMatrixPhysical()

	position.structSize     = 105;
	position.mask           = 14;
	position.state          = 0;
	position.transformation = [0, 0, 0, 0; 0, 0, 0, 0; 0, 0, 0, 0; 0, 0, 0, 0 ];
	position.maxSpeed       = 65535;
	position.triggers       = 0;
	position.aux            = [ 0, 0, 0, 0, 0, 0, 0, 0 ];

	out = position;

end
