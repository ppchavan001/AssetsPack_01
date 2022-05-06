// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeEditorFunctionsBPLibrary.h"
#include "ExposeEditorFunctions.h"
#include <DisplayCluster/Public/Blueprints/DisplayClusterBlueprint.h>
#include <DisplayClusterConfigurator/Private/DisplayClusterConfiguratorEditorSubsystem.h>
#include "Modules/ModuleManager.h"
#include "IDisplayClusterConfiguration.h"
#include <DisplayClusterConfigurator/Private/DisplayClusterConfiguratorModule.h>
#include "DesktopPlatform/Public/DesktopPlatformModule.h"





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

bool UExposeEditorFunctionsBPLibrary::ExportNDisplayConfigFromDisplayClusterRootActorBlueprint(UObject* Object, const FString& FilePath)
{

	UDisplayClusterBlueprint* Blueprint = Cast<UDisplayClusterBlueprint>(Object);

	if (Blueprint)
	{
		Blueprint->PrepareConfigForExport();

		UDisplayClusterConfigurationData* ConfigData = Blueprint->GetConfig();

		IDisplayClusterConfiguration& Module = FModuleManager::GetModuleChecked<IDisplayClusterConfiguration>("DisplayClusterConfiguration");


		if (ConfigData)	return Module.SaveConfig(ConfigData, FilePath);
		
		
	}


	return false;
}

FText UExposeEditorFunctionsBPLibrary::GetDisplayClusterExportConfigPathFromBlueprintInternal(UObject* Object, bool& ReturningValidPath)
{
	UE_LOG(LogTemp, Warning, TEXT("-------------------------------------------------------------------------------------------"));
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
			ReturningValidPath = true;
			return FText::FromString(ConfigPath);
		}
		ReturningValidPath = false;
		return FText::FromString("Path is not set for selected Blueprint. Open the Blueprint and Export to a valid location to set Path.");
	}

	ReturningValidPath = false;
	return FText::FromString("Selected object is not a Display Cluster Blueprint.");
}

FString UExposeEditorFunctionsBPLibrary::OpenSelectDirectoryDialog(FString DefaultDirectory)
{
	FString OutputDirectory;

	if (FDesktopPlatformModule::Get()->OpenDirectoryDialog(FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr), ("Select Export Directory"), DefaultDirectory, OutputDirectory))
	{
		return OutputDirectory;
	}


	return FString();
}



FText UExposeEditorFunctionsBPLibrary::GetDisplayClusterExportConfigPathFromBlueprint(UObject* Object)
{
	bool bReturnedValidPath = false;
	FText text = GetDisplayClusterExportConfigPathFromBlueprintInternal(Object, bReturnedValidPath);


	if (bReturnedValidPath)
	{
		return text;
	}
	else
	{


		// Get the user's temp directory
		FString TargetPath = FDesktopPlatformModule::Get()->GetUserTempPath();
		TargetPath.Append(Object->GetName());
		TargetPath.Append(".ndisplay");

		bool bExported =
			UExposeEditorFunctionsBPLibrary::ExportNDisplayConfigFromDisplayClusterRootActorBlueprint(Object, TargetPath);

		if (bExported)
		{
			return FText::FromString(TargetPath);
		}

		return FText::FromString("UExposeEditorFunctionsBPLibrary::GetDisplayClusterExportConfigPathFromBlueprint : Error exporting data!");


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


