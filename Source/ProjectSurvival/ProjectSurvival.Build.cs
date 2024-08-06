// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class ProjectSurvival : ModuleRules
{
    public ProjectSurvival(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Slate", "SlateCore", "HeadMountedDisplay", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam", "Steamworks", "Networking", "Sockets", "ApexDestruction" });
        //PublicIncludePaths.AddRange(new string[] { "ApexDestruction/Public", "ApexDestruction/Private" });

        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemUtils" });
        PrivateIncludePaths.Add("ProjectSurvival");
        // Steam SDK 경로 설정 
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string SteamLibPath = Path.Combine(ModuleDirectory, "../../ThirdParty/Steamworks/Steamv147/sdk/redistributable_bin/win64");
            PublicAdditionalLibraries.Add(Path.Combine(SteamLibPath, "steam_api64.lib"));
            RuntimeDependencies.Add(Path.Combine(SteamLibPath, "steam_api64.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            string SteamLibPath = Path.Combine(ModuleDirectory, "../../ThirdParty/Steamworks/Steamv147/sdk/redistributable_bin");
            PublicAdditionalLibraries.Add(Path.Combine(SteamLibPath, "steam_api.lib"));
            RuntimeDependencies.Add(Path.Combine(SteamLibPath, "steam_api.dll"));
        }
    }
}
