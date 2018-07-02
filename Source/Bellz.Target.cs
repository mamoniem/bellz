// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class BellzTarget : TargetRules
{
	public BellzTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
		
		ExtraModuleNames.Add("Bellz");
	}

	//
	// TargetRules interface.
	//
/*
	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "Bellz" } );
	}
	*/
}
