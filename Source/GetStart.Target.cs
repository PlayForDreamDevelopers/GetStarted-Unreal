// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GetStartTarget : TargetRules
{
	public GetStartTarget( TargetInfo Target) : base(Target)
	{
        bOverrideBuildEnvironment = true;
        Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		ExtraModuleNames.AddRange( new string[] { "GetStart" } );
	}
}
