// Copyright 2024 3D Source, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PakExportRuntime : ModuleRules
{
	public PakExportRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"LevelSequence"
				// ... add other public dependencies that you statically link with here ...
			}
			);


		PrivateIncludePathModuleNames.AddRange(
			new string[]
			{

			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"CinematicCamera",
				"Json",
				"JsonUtilities",
				"LevelSequence",
				"PakFile"
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
				
			}
			);

		CppStandard = CppStandardVersion.Cpp17;
	}
}
