using UnrealBuildTool;

public class TablePhyPos_Unreal : ModuleRules
{
	public TablePhyPos_Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ForceSeatMI" });

		MinFilesUsingPrecompiledHeaderOverride = 1;
		bFasterWithoutUnity = true;
	}
}
