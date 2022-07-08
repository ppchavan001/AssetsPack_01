// Some copyright should be here...

using UnrealBuildTool;

public class nDisplayRenderRuntime : ModuleRules
{
	public nDisplayRenderRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "DisplayCluster",
                "Engine",
                "Projects",
                "UMG",
                "InputCore"
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DisplayCluster",
                 "DisplayClusterConfiguration",
                "LevelSequence",
                "MovieScene"
				// ... add private dependencies that you statically link with here ...	


				 
            }
            );

        PublicIncludePathModuleNames.Add("DisplayClusterConfigurator");
        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
