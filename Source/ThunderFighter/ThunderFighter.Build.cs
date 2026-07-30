// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ThunderFighter : ModuleRules
{
	public ThunderFighter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// 添加模块子目录到 include path，使跨目录 #include 可以正常解析
		PublicIncludePaths.AddRange(new string[] {
			ModuleDirectory,
			ModuleDirectory + "/Core",
			ModuleDirectory + "/Actors",
			ModuleDirectory + "/Components",
			ModuleDirectory + "/UI"
		});

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// 若使用 Slate UI 则取消注释
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// 若使用在线功能则取消注释
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// 要包含 OnlineSubsystemSteam 的话，将其添加到 uproject 文件的 plugins 节并将 Enabled 属性设为 true
	}
}
