function out = ForceSeatMI_GetPlatformInfoEx(api)
    info = FSMI_PlatformInfo();
    info_C = libstruct('FSMI_PlatformInfo', info);

    if (0 < calllib(FSMI_Dll(),'ForceSeatMI_GetPlatformInfoEx', api, info_C, info.structSize, 100))
        out = get(info_C);
    else
        % Always return something
        out = get(info_C);
    end

    clear info_C;
end