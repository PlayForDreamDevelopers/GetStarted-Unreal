// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.
using System;
using System.IO;

namespace UnrealBuildTool.Rules
{
    public class YvrMobile : ModuleRules
    {
        public YvrMobile(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            string EngineRuntimePath = Path.GetFullPath(Target.RelativeEnginePath + "/Source/Runtime");

            PrivateIncludePaths.AddRange(
                new string[] {
                "YvrMobile/Private",
                PluginDirectory + "/Include",
                EngineRuntimePath + "/Renderer/Private",
                EngineRuntimePath + "/Core/Private",
                EngineRuntimePath + "/Launch/Private",
                EngineRuntimePath + "/OpenGLDrv/Private",
            });

            if (Target.Type == TargetRules.TargetType.Editor)
            {
                PrivateDependencyModuleNames.AddRange(
                    new string[]
                    {
                        "UnrealEd",
                        "Slate",
                        "SlateCore",
                        "EditorStyle",
                        "EditorWidgets",
                        "DesktopWidgets",
                        "PropertyEditor",
                        "SharedSettingsWidgets",
                        "SequencerWidgets"
                    }
                );
            }

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "RHI",
                    "RenderCore",
                    "Renderer",
                    "HeadMountedDisplay",
                    "Json",
                    "JsonUtilities",
					"ProceduralMeshComponent",
                    "Projects",
                }
            );

            PublicIncludePathModuleNames.AddRange(
                new string[]
                {
                    "Launch",
                    "ProceduralMeshComponent",
                }
            );
            PrivateDependencyModuleNames.AddRange(new string[] { "OpenGLDrv" });
            AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenGL");

            if (Target.Platform == UnrealTargetPlatform.Android)
            {
                PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
                string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "YvrMobileHMD_APL.xml"));
                PublicAdditionalLibraries.Add(PluginDirectory + "/Libs/arm64-v8a/libyvrlib.so");
                PublicAdditionalLibraries.Add(PluginDirectory + "/Libs/armeabi-v7a/libyvrlib.so");
            }
        }
    }
}
