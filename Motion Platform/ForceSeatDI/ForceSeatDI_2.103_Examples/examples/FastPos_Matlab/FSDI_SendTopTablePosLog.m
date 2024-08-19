function out = FSDI_SendTopTablePosLog(api, position)
    out = calllib(FSDI_Dll(),'ForceSeatDI_SendTopTablePosLog', api, position);
end
