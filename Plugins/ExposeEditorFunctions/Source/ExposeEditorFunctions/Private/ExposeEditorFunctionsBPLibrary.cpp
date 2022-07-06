// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeEditorFunctionsBPLibrary.h"
#include "ExposeEditorFunctions.h"
#include <DisplayClusterConfigurator/Private/DisplayClusterConfiguratorEditorSubsystem.h>
#include "Modules/ModuleManager.h"
#include "IDisplayClusterConfiguration.h"
#include <DisplayClusterConfigurator/Private/DisplayClusterConfiguratorModule.h>
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Components/ComboBoxString.h"
#include "Logging/LogMacros.h"

#if EEFBPL_ENGINE_VERSION_MAJOR > 426

#include <DisplayCluster/Public/Blueprints/DisplayClusterBlueprint.h>

#endif	

#if EEFBPL_ENGINE_VERSION_MAJOR >= 500

#include "EditorScriptingHelpers.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Subsystems/EditorActorSubsystem.h"


#endif 

#if EEFBPL_ENGINE_VERSION_MAJOR == 427

#include "EditorLevelLibrary.h"

#endif // EEFBPL_ENGINE_VERSION_MAJOR == 427



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





#if EEFBPL_ENGINE_VERSION_MAJOR > 426
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

#else

FText UExposeEditorFunctionsBPLibrary::GetDisplayClusterExportConfigPathFromBlueprintInternal(UObject* Object, bool& ReturningValidPath)
{
	return FText::FromString(" Unreal version lower than 4.27 are not supported.");
}

bool UExposeEditorFunctionsBPLibrary::ExportNDisplayConfigFromDisplayClusterRootActorBlueprint(UObject* Object, const FString& FilePath)
{

	return false;
}

#endif


FString UExposeEditorFunctionsBPLibrary::OpenSelectDirectoryDialog(FString DefaultDirectory)
{
	FString OutputDirectory;

	if (FDesktopPlatformModule::Get()->OpenDirectoryDialog(FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr), ("Select Export Directory"), DefaultDirectory, OutputDirectory))
	{
		return OutputDirectory;
	}


	return FString();
}

void UExposeEditorFunctionsBPLibrary::SetComboBoxStringFont(UComboBoxString* ComboBoxToUpdate, FSlateFontInfo NewFont)
{
	ComboBoxToUpdate->Font = NewFont;

	// TODO : Refresh widget
	//ComboBoxToUpdate->SetSelectedOption(ComboBoxToUpdate->GetSelectedOption());
}



#if EEFBPL_ENGINE_VERSION_MAJOR > 500 

UWorld* UExposeEditorFunctionsBPLibrary::GetEditorWorld()
{
	UUnrealEditorSubsystem* UnrealEditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();

	if (!UnrealEditorSubsystem || !EditorScriptingHelpers::CheckIfInEditorAndPIE())
	{
		return nullptr;
	}


	UWorld* World = UnrealEditorSubsystem->GetEditorWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s. Can't spawn the actor because there is no world."));
		return nullptr;
	}

	return World;
}

TArray<class AActor*> UExposeEditorFunctionsBPLibrary::GetAllLevelActors()
{
	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();

	return EditorActorSubsystem ? EditorActorSubsystem->GetAllLevelActors() : TArray<AActor*>();
}

#else

#if EEFBPL_ENGINE_VERSION_MAJOR > 426
UWorld* UExposeEditorFunctionsBPLibrary::GetEditorWorld()
{
	return UEditorLevelLibrary::GetEditorWorld();
}

TArray<class AActor*> UExposeEditorFunctionsBPLibrary::GetAllLevelActors()
{
	return UEditorLevelLibrary::GetAllLevelActors();
}
#else
UWorld* UExposeEditorFunctionsBPLibrary::GetEditorWorld()
{
	return nullptr;
}


TArray<class AActor*> UExposeEditorFunctionsBPLibrary::GetAllLevelActors()
{
	return TArray<class AActor*>();
}
#endif

#endif


#if EEFBPL_ENGINE_VERSION_MAJOR > 426

FText UExposeEditorFunctionsBPLibrary::GetDisplayClusterExportConfigPathFromBlueprint(UObject* Object, const bool bForceExport)
{
	bool bReturnedValidPath = false;
	FText text = GetDisplayClusterExportConfigPathFromBlueprintInternal(Object, bReturnedValidPath);


	if (bReturnedValidPath && !bForceExport)
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

#else

FText UExposeEditorFunctionsBPLibrary::GetDisplayClusterExportConfigPathFromBlueprint(UObject* Object)
{
	return FText::FromString(" Unreal version lower than 4.27 are not supported.");
}


FString UExposeEditorFunctionsBPLibrary::GetConfigData(UObject* Object)
{
	return (" Unreal version lower than 4.27 are not supported.");
}

#endif
