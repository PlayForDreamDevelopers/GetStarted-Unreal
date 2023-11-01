// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.
using System;
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class YvrInput : ModuleRules
	{
		public YvrInput(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            string EngineRuntimePath = Path.GetFullPath(Target.RelativeEnginePath + "/Source/Runtime");

            PrivateIncludePathModuleNames.AddRange(
                new string[]
                {
                    "InputDevice",			// For IInputDevice.h
					"HeadMountedDisplay",	// For IMotionController.h
					"ImageWrapper"
                });

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "ApplicationCore",
                    "Engine",
                    "InputCore",
                    "HeadMountedDisplay",
                    "YvrMobile",
                });

            PrivateIncludePaths.AddRange(
                new string[] {
                    "YvrMobile/Private",
                    PluginDirectory + "/Include",
                });


        }
    }
}
