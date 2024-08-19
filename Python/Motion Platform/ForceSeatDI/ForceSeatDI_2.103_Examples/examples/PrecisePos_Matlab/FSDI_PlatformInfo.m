function out = FSDI_PlatformInfo()

out.structSize                   = uint32(FSDI_Constants.Size_FSDI_PlatformInfo);
out.state                        = 0;
out.isThermalProtectionActivated = 0;
out.coolingSystemMalfunction     = 0;
out.moduleStatus                 = [ 0, 0, 0, 0, 0, 0 ];
out.serialNumber                 = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ];

end
