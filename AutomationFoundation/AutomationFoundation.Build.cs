// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AutomationFoundation : ModuleRules
{
	public AutomationFoundation(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "CommonUI", "AIModule", "Niagara" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags" });
	}
}