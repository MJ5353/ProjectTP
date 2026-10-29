// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectTP : ModuleRules
{
	public ProjectTP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",

			// Input
			"EnhancedInput",

			// GAS
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",

			// AI
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",

			// UMG
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Epic 템플릿 코드가 짧은 include를 써서 남겨 둔다. 내 코드는 "ProjectTP/..." 전체 경로로 include한다.
		PublicIncludePaths.AddRange(new string[] {
			"ProjectTP",
			"ProjectTP/Variant_Combat",
			"ProjectTP/Variant_Combat/AI",
			"ProjectTP/Variant_Combat/Animation",
			"ProjectTP/Variant_Combat/Gameplay",
			"ProjectTP/Variant_Combat/Interfaces",
			"ProjectTP/Variant_Combat/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
