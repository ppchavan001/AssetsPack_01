// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#define EEFBPL_ENGINE_VERSION_MAJOR 4

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExposeEditorFunctionsBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

class UComboBoxString;

UCLASS()
class UExposeEditorFunctionsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

#if WITH_EDITOR

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Lightmap Resolution for Target Mesh", Keywords = "set lightmap resolution"), Category = "Exposed Functions | Lighting")
		static void SetStaticMeshLightMapResolution(UStaticMeshComponent* TargetMesh, int NewRes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Display Cluster Export Config Path From Blueprint", Keywords = "Get Display Cluster Export Config Path"), Category = "Exposed Functions | NDisplay | Display Cluster Blueprint")
		static FText GetDisplayClusterExportConfigPathFromBlueprint(UObject* Object);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Display Cluster Config", Keywords = "Get Display Cluster  Config "), Category = "Exposed Functions | NDisplay | Display Cluster Blueprint")
	static FString GetConfigData(UObject* Object);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Display Cluster Config", Keywords = "Get Display Cluster  Config "), Category = "Exposed Functions | NDisplay | Display Cluster Blueprint")
		static bool ExportNDisplayConfigFromDisplayClusterRootActorBlueprint(UObject* Object, const FString& FilePath);

	static FText GetDisplayClusterExportConfigPathFromBlueprintInternal(UObject* Object, bool& ReturningValidPath);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Open Select Directory Dialog", Keywords = "Open Select Directory Dialog"), Category = "Exposed Functions | File Manager")
		static FString OpenSelectDirectoryDialog(FString DefaultDirectory);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set ComboBoxString Font", Keywords = "Set ComboBoxString Font"), Category = "Exposed Functions | Widgets")
		static void SetComboBoxStringFont(UComboBoxString* ComboBoxToUpdate, FSlateFontInfo NewFont);

	UFUNCTION(BlueprintCallable, Category = "Exposed Functions | Level Utility")
		static UWorld* GetEditorWorld();

	UFUNCTION(BlueprintCallable, Category = "Exposed Functions | Level Utility")
		static TArray<class AActor*> GetAllLevelActors();

#endif
};

