// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DylanAdventure : ModuleRules
{
	public DylanAdventure(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayTasks", "UMG" });
	}
}
