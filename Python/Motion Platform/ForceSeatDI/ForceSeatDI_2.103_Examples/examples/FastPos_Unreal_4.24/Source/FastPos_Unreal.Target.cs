using UnrealBuildTool;
using System.Collections.Generic;

public class FastPos_UnrealTarget : TargetRules
{
	public FastPos_UnrealTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.AddRange( new string[] { "FastPos_Unreal" } );
	}
}
