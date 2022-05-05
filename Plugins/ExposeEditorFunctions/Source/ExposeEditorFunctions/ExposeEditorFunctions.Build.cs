// Some copyright should be here...

using UnrealBuildTool;

public class ExposeEditorFunctions : ModuleRules
{
	public ExposeEditorFunctions(ReadOnlyTargetRules Target) : base(Target)
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
                "CoreUObject",
                "DisplayCluster",
                "Engine",
                "Projects",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"Slate",
				"SlateCore",
				"DisplayCluster",
				"DisplayClusterEditor",
				 "DisplayClusterConfiguration",
				"DisplayClusterConfigurator"
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
