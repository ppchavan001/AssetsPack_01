// Copyright Epic Games, Inc. All Rights Reserved.

#include "nDisplayRenderBPLibrary.h"
#include "nDisplayRender.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Blueprints/DisplayClusterBlueprint.h"
#include "IDisplayClusterConfiguration.h"


UnDisplayRenderBPLibrary::UnDisplayRenderBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


FString UnDisplayRenderBPLibrary::OpenSelectDirectoryDialog(FString DefaultDirectory)
{
	FString OutputDirectory;

	if (FDesktopPlatformModule::Get()->OpenDirectoryDialog(FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr), ("Select Export Directory"), DefaultDirectory, OutputDirectory))
	{
		return OutputDirectory;
	}


	return FString();
}


FText UnDisplayRenderBPLibrary::GetDisplayClusterExportConfigPathFromBlueprintInternal(UObject* Object, bool& ReturningValidPath)
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


FText UnDisplayRenderBPLibrary::GetDisplayClusterExportConfigPathFromBlueprint(UObject* Object, const bool bForceExport)
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
			UnDisplayRenderBPLibrary::ExportNDisplayConfigFromDisplayClusterRootActorBlueprint(Object, TargetPath);

		if (bExported)
		{
			return FText::FromString(TargetPath);
		}

		return FText::FromString("UExposeEditorFunctionsBPLibrary::GetDisplayClusterExportConfigPathFromBlueprint : Error exporting data!");


	}
}


bool UnDisplayRenderBPLibrary::ExportNDisplayConfigFromDisplayClusterRootActorBlueprint(UObject* Object, const FString& FilePath)
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

void UnDisplayRenderBPLibrary::SetComboBoxStringFont(UComboBoxString* ComboBoxToUpdate, FSlateFontInfo NewFont)
{
	ComboBoxToUpdate->Font = NewFont;

	// TODO : Refresh widget
	//ComboBoxToUpdate->SetSelectedOption(ComboBoxToUpdate->GetSelectedOption());
}



