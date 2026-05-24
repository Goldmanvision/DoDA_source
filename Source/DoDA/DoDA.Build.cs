// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DoDA : ModuleRules
{
    public DoDA(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
        });
    }
}