// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeEditorFunctionsBPLibrary.h"
#include "ExposeEditorFunctions.h"
#include <DisplayCluster/Public/Blueprints/DisplayClusterBlueprint.h>
#include <DisplayClusterConfigurator/Private/DisplayClusterConfiguratorEditorSubsystem.h>
#include "Modules/ModuleManager.h"
#include "IDisplayClusterConfiguration.h"
#include <DisplayClusterConfigurator/Private/DisplayClusterConfiguratorModule.h>





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

FText UExposeEditorFunctionsBPLibrary::GetDisplayClusterExportConfigPathFromBlueprintInternal(UObject* Object, bool& ReturningValidPath)
{
	
	return FText::FromString("Selected object is not a Display Cluster Blueprint.");

}

FText UExposeEditorFunctionsBPLibrary::GetDisplayClusterExportConfigPathFromBlueprint(UObject* Object)
{
	bool bReturnedValidPath = false;
	FText text = GetDisplayClusterExportConfigPathFromBlueprintInternal(Object, bReturnedValidPath);


	UE_LOG(LogTemp, Warning, TEXT("75"));


	if (bReturnedValidPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("80"));
		return text;
	}
	else
	{
		UDisplayClusterBlueprint* Blueprint = Cast<UDisplayClusterBlueprint>(Object);
		Blueprint->GetConfig();
		//auto Data = Blueprint->GetConfig();
		//UDisplayClusterConfiguratorEditorSubsystem* sub =	GEditor->GetEditorSubsystem<UDisplayClusterConfiguratorEditorSubsystem>();
		//FDisplayClusterConfiguratorBlueprintEditor* SelectedBlueprintEditor = new FDisplayClusterConfiguratorBlueprintEditor();
		//SelectedBlueprintEditor = FDisplayClusterConfiguratorUtils::GetBlueprintEditorForObject(Object);

		//
		//if (SelectedBlueprintEditor)
		//{
		//
		//	if (SelectedBlueprintEditor->CanExportConfig())
		//	{
		//		// Get the user's temp directory
		//		FString UserTempDir = FDesktopPlatformModule::Get()->GetUserTempPath();
		//		UserTempDir.Append("\\");
		//		UserTempDir.Append(Object->GetName());
		//		SelectedBlueprintEditor->SaveToFile(UserTempDir);
		//
		//		UE_LOG(LogTemp, Warning, TEXT("101"));
		//
		//		return FText::FromString(UserTempDir);
		//	}
		//}
		//
		UE_LOG(LogTemp, Warning, TEXT("108"));
		return text;

	}
}

FString UExposeEditorFunctionsBPLibrary::GetConfigData(UObject* Object)
{
	UDisplayClusterBlueprint* Blueprint = Cast<UDisplayClusterBlueprint>(Object);

	if (Blueprint)
	{
		Blueprint->PrepareConfigForExport();

		UDisplayClusterConfigurationData* ConfigData = Blueprint->GetConfig();

		IDisplayClusterConfiguration& Module = FModuleManager::GetModuleChecked<IDisplayClusterConfiguration>("DisplayClusterConfiguration");

		//Mod.SaveConfig()
		//IDisplayClusterConfigurator& Mod2 = FModuleManager::GetModuleChecked<IDisplayClusterConfigurator>("DisplayClusterConfigurator");


		if (ConfigData)
		{
			FString s;
			Module.ConfigAsString(ConfigData, s);
			return s;
		}
		return FString("Invalid data in the blueprint.");
	}

	return FString("Invalid blueprint passed.");
}



