// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Fighting : ModuleRules
{
	public Fighting(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "Niagara", "NavigationSystem", "AkAudio", "WwiseSoundEngine"  });
	}
}
