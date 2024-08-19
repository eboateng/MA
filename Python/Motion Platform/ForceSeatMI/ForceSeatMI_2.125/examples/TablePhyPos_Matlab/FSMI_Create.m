function api = FSMI_Create()
    if not(libisloaded(FSMI_Dll()))
        loadlibrary(FSMI_Path(), 'include/ForceSeatMI_Functions.h')
    end

    api = calllib(FSMI_Dll(), 'ForceSeatMI_Create');
end

