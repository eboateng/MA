classdef FSDI_Constants < uint32
    enumeration
        Size_FSDI_ActualActuatorsPositionLogical(65)
        Size_FSDI_ActualTopTablePositionPhysical(29)
        Size_FSDI_TopTablePositionLogical(20)
        Size_FSDI_TopTablePositionPhysical(33)
        Size_FSDI_PlatformInfo(25)
        Size_SerialNumber(28)
        
        Mask_FSDI_TopTablePositionLogical(7)   %Sets pause, position and speed bits
        Mask_FSDI_TopTablePositionPhysical(15) %Sets pause, position , strategy and speed bits
        
        Max_Speed(65535)
    end
end
