// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OldHouse : ModuleRules
{
	public OldHouse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
			"GameplayTasks", "Paper2D" });
	}
}
