function FSMI_ActivateProfile(api, profile)
    calllib(FSMI_Dll(),'ForceSeatMI_ActivateProfile', api, profile); 
end

