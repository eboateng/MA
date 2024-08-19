function out = FSDI_TopTablePositionLogical()

out.structSize = uint32(FSDI_Constants.Size_FSDI_TopTablePositionLogical);
out.mask       = uint32(FSDI_Constants.Mask_FSDI_TopTablePositionLogical);
out.pause      = 0;
out.roll       = 0;
out.pitch      = 0;
out.yaw        = 0;
out.heave      = 0;
out.sway       = 0;
out.surge      = 0;
out.maxSpeed   = uint32(FSDI_Constants.Max_Speed);

end
