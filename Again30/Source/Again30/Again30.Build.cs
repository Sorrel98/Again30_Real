// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Again30 : ModuleRules
{
	public Again30(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
