// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Skate : ModuleRules
{
	public Skate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"GameplayTags"
		});
	}
}
