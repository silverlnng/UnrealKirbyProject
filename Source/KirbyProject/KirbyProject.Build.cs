// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KirbyProject : ModuleRules
{
	public KirbyProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem", "AIModule" });
	}
}
