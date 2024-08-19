using UnrealBuildTool;

public class FastPos_Unreal : ModuleRules
{
	public FastPos_Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ForceSeatDI" });

		MinFilesUsingPrecompiledHeaderOverride = 1;
		bUseUnity = true;
	}
}
