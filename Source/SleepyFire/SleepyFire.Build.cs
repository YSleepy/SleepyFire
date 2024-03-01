// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SleepyFire : ModuleRules
{
	public SleepyFire(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
			"SleepyFire"
			}
);
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",
			"ModularGameplay",
			"GameFeatures",
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"InputCore",
			"EnhancedInput",
			"GameplayMessageRuntime",
			"LoadingScreen"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
