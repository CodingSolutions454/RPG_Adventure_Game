// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyProject : ModuleRules
{
	public MyProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" , "AIModule"});
	}
}
