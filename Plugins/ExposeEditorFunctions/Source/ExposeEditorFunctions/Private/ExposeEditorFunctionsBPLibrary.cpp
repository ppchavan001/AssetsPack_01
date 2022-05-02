// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeEditorFunctionsBPLibrary.h"
#include "ExposeEditorFunctions.h"

#include "../Plugins/Runtime/nDisplay/Source/DisplayClusterConfiguration/Public/DisplayClusterConfigurationStrings.h"
#include "../Plugins/Runtime/nDisplay/Source/DisplayCluster/Public/Blueprints/DisplayClusterBlueprint.h"

//#include "DisplayClusterConfigurationStrings.h"
//#include "Blueprints/DisplayClusterBlueprint.h"

UExposeEditorFunctionsBPLibrary::UExposeEditorFunctionsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}



void UExposeEditorFunctionsBPLibrary::SetStaticMeshLightMapResolution(UStaticMeshComponent* TargetMesh, int NewRes)
{
	TargetMesh->bOverrideLightMapRes = 1;
	TargetMesh->OverriddenLightMapRes = NewRes;
	TargetMesh->InvalidateLightingCacheDetailed(true, false);
}

FText UExposeEditorFunctionsBPLibrary::GetDisplayClusterExportConfigPathFromBlueprint(UObject* Object)
{
    //const FString CorrectExtension = DisplayClusterConfigurationStrings::file::FileExtJson;
    if (UDisplayClusterBlueprint* Blueprint = Cast<UDisplayClusterBlueprint>(Object))
    {
        FString ConfigPath = Blueprint->GetConfigPath();
        if (!ConfigPath.IsEmpty())
        {
            const FString CorrectExtension = DisplayClusterConfigurationStrings::file::FileExtJson;

            if (FPaths::GetExtension(ConfigPath) != CorrectExtension) 
            {
                ConfigPath = FPaths::ChangeExtension(ConfigPath, CorrectExtension);
            }

            if (FPaths::IsRelative(ConfigPath)) 
            {
                ConfigPath = FPaths::ConvertRelativePathToFull(ConfigPath);
            }

            return FText::FromString(ConfigPath);
        }
        return FText::FromString("No path set for current Blueprint - Export config to set a path.");
    }
    
    return FText::FromString("Selected object is not a Display Cluster Blueprint.");
}

