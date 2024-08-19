using UnrealBuildTool;

public class Telemetry_Fly_Unreal : ModuleRules
{
	public Telemetry_Fly_Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ForceSeatMI", "ForceSeatMIPlane" });

		MinFilesUsingPrecompiledHeaderOverride = 1;
		bFasterWithoutUnity = true;
	}
}
