// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputCore/Classes/InputCoreTypes.h"
#include "UObject/UnrealType.h"
#include "UObject/UObjectIterator.h"
#include "Engine/EngineBaseTypes.h"
#include "GameFramework/Actor.h"

#include <Runtime\Projects\Public\Interfaces\IPluginManager.h>
#include <Runtime\Projects\Public\PluginDescriptor.h>
#include "DataFactoryBPLibrary.generated.h"




UENUM(BlueprintType)
enum class EDataFactoryLogVerbosity : uint8
{
	/** returns without logging */
	NoLogging = 0,

	/** Always prints a fatal error to console (and log file) and crashes (even if logging is disabled) */
	Fatal,

	/**
		* Prints an error to console (and log file).
		* Commandlets and the editor collect and report errors. Error messages result in commandlet failure.
		*/
		Error,

		/**
		* Prints a warning to console (and log file).
		* Commandlets and the editor collect and report warnings. Warnings can be treated as an error.
		*/
		Warning,

		/** Prints a message to console (and log file) */
		Display,

		/** Prints a message to a log file (does not print to console) */
		Log,

		/**
		* Prints a verbose message to a log file (if Verbose logging is enabled for the given category,
		* usually used for detailed logging)
		*/
		Verbose,

		/**
		* Prints a verbose message to a log file (if VeryVerbose logging is enabled,
		* usually used for detailed logging that would otherwise spam output)
		*/
		VeryVerbose

};
#define GETENUMSTRING(etype, evalue) ( (FindObject(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )


UENUM(BlueprintType)
enum class EInputBindingSupportedTypes : uint8
{
	ActionBinding,
	KeyBinding,
	AxisBinding,
	Invalid
};


UCLASS()
class DATAFACTORY_API UDataFactoryBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Key From Name", Keywords = "Get Key From Name"), Category = "DataFactory | Input")
		static FKey GetKeyFromName(FName name);


	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, Keywords = "log print", AdvancedDisplay = "2", DevelopmentOnly), Category = "DataFactory | Log")
		static void DF_PrintString(const UObject* WorldContextObject,
			const FString InString = "",
			EDataFactoryLogVerbosity LogVerbosity = EDataFactoryLogVerbosity::Log,
			bool bPrintToScreen = true,
			bool bPrintToLog = true,
			float Duration = 2.f,
			int MaxStackDataDepth = 1);

	//UFUNCTION(BlueprintCallable, meta = (Keywords = "log print", DevelopmentOnly), Category = "DataFactory | Log")
	static void DF_PrintClass(const UObject* WorldContextObject,
		EDataFactoryLogVerbosity LogVerbosity = EDataFactoryLogVerbosity::Log,
		bool bPrintToScreen = true,
		bool bPrintToLog = true,
		float Duration = 2.f);


	/*
		Finds the property by name on the specified object and updates the value.

		Supported property types :
		BoolProperty		: valid values -> "true" or "false",
		NameProperty		: valid values ->  any string will be converted to name,
		StrProperty			: valid values ->  any string,
		TextProperty		: valid values -> Any string will be converted to text,
		EnumProperty		: Type = Uint8 : valid values -> 0 to (255 or max Enum size which ever is lower),
		ArrayProperty		: array of all supported types : valid values -> Comma separated values of the supported type,
		NumericProperty		: all numeric types : ex float, int, double, byte
		StructProperty		:
					//	Color					: Type = Vec4 of Uint8 :  comma separated keys ex. R:0, G: 25, B:127, A:255 or CSV in same order
					//	LinearColor				: Type = Vec4 of float :  comma separated keys ex. R:1110, G: 425, B:1527, A:255 or CSV in same order
					//	Vector(location, scale)	: Type = Vec3 of float :   comma separated keys ex. LocX: 125, LocY: 778, LocZ : -220 or CSV in same order
					//	Rotator					: Type = Vec4 of float :   comma separated keys ex. RotX : -75, RotY: 76, RotZ: -650 or CSV in same order
					//	Transform				: Type = 2 Vector + 1 Rotator :   comma separated keys ex. LocX: 400,  RotZ: -100,   ScaleX: 12.2, ScaleY : -54, ScaleZ: 70
					//								or CSV in Loc(x,y,z), Rotation(x,y,z), Scale(x,y,z)


		Types that are not supported:
		MapProperty,
		SetProperty,
		ObjectPropertyBase,
		DelegateProperty,
		InterfaceProperty,
		MulticastDelegateProperty,
		NetPropertyHook


	*/
	UFUNCTION(BlueprintCallable, Category = "DataFactory | General", meta = (DisplayName = "Set Property Value By Name"))
		static void SetFPropertyByName(UObject* Object, FName NameOfThePropertyToUpdate, const FString DataToSet);


	// Called from SetFPropertyByName
	static void SetFPropertyValueInternal(FProperty* property, void* Object, const FString DataToSet, FName NameOfThePropertyToUpdate);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Property Class Name"), Category = "DataFactory | General")
		static FString GetFPropertyClassName(UObject* Object, FName PropertyName);

	// returns object with name
	static UObject* GetObjectWithName(const FName Name);

	// Get all objects with one of the names in the set
	// if you are calling "GetObjectWithName" multiple times with different names, 
	// you can use this as an optimization
	static TSet<UObject*> GetObjectsWithNames(const TSet<FName>& ObjectNames);


	UFUNCTION(BlueprintPure, Category = "DataFactory | General")
		static void GetAllObjects(TSet<UObject*>& SetOfObjectsOut);


	// Return true if valid data is found
	UFUNCTION(BlueprintPure, Category = "DataFactory | General")
		static bool GetPluginDescription(const FString PluginName, FString& PluginVersion, FString& AppBuildDate, FString& AppBuildVersion);





	/// <summary>
	// Removes existing action/axis/key bindings and binds the function to it.
	//	
	// Object = Container of the function
	// SourceName = Action name/ axis name / key to bind function to
	// KeyEvent = Trigger type. Only used in action/ key bindings
	// InputComponent = Component to use to update bindings
	//		if InputComponent is not provided, player0 Controller's InputComponent will be used for binding 
	// 
	// Returns Bool : true if succeeded in binding function false otherwise
	// 
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "DataFactory | Input")
		static bool AddInputBinding(UObject* Object,
			FName SourceName,
			FName FunctionName,
			EInputBindingSupportedTypes InputBindingType,
			EInputEvent KeyEvent = IE_Released,
			UInputComponent* InputComponent = nullptr);

	static void BindActionInputInternal(UInputComponent* InputComponent, const FName& ActionName, UObject* Object, FName& FunctionName, EInputEvent KeyEvent);
	static void BindAxisInputInternal(UInputComponent* InputComponent, const FName& AxisName, UObject* Object, FName& FunctionName);
	static void BindKeyInputInternal(UInputComponent* InputComponent, const FName& KeyName, UObject* Object, FName& FunctionName, EInputEvent KeyEvent);


	UFUNCTION(BlueprintCallable, Category = "DataFactory | Import/ Export", meta = (DisplayName = "WriteStringToFile"))
		static bool WriteStringToFile(const FString FileName, const FString DataToWrite);

	UFUNCTION(BlueprintPure, Category = "DataFactory | Import/ Export", meta = (DisplayName = "ReadLinesFromFile"))
		static bool ReadLinesFromFile(const FString FileName, TArray<FString>& LinesOut);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DataFactory | Import/ Export", meta = (DisplayName = "ReadStringFromFile"))
		static bool ReadStringFromFile(const FString FileName, FString& StringOut);
	/*
	* Only checks for first Char in char for validation
	*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "Final all char"), Category = "DataFactory | String")
		static void GetAllIndicesOfCharInString(const FString& String, const FString& Char, TArray<int32>& ArrOfIndices);

	UFUNCTION(BlueprintPure, Category = "DataFactory | General")
		static FString GetAppInfo(FString Separator = "");

	// if actor doesn't exist in the world of WorldContextObject,
	// will spawn a actor of given class and return it at index 0
	UFUNCTION(BlueprintPure, Category = "DataFactory | General", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "ActorClass", DynamicOutputParam = "OutActors"))
		static void GetAllActorsOfClass_Forced(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "TargetClass", DynamicOutputParam = "FoundSubClasses"), Category = "DataFactory | General")
		static void GetAllBlueprintClassesOfType(TArray<UClass*>& FoundSubClasses,
			TSubclassOf<UObject> TargetClass /*= UObject::StaticClass()*/,
			bool bIncludeTargetClass = false, bool bSerchProjectPlugins = true, bool bSearchEnginePlugins = true);

	// Returns all content directories from all enabled plugins in the project
	UFUNCTION(BlueprintPure, Category = "DataFactory | General")
	static void GetAllContentDir(TArray<FString>& ContentDir);

	// Returns all all enabled plugins in the project with content
	UFUNCTION(BlueprintPure, Category = "DataFactory | General")
	static void GetAllEnabledPluginWithContentNames(TArray<FString>& PluginNames, bool bSerchProjectPlugins = true, bool bSearchEnginePlugins = true);

	// Finds first component with matching tag or name
	// tag has higher priority
	UFUNCTION(BlueprintPure, Category = "DataFactory | General")
		static USceneComponent* FindFirstSceneComponentByName(const UObject* WorldContextObject, FName ComponentName);

};

