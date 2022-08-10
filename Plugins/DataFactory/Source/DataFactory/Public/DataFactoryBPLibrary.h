// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputCore/Classes/InputCoreTypes.h"
#include "UObject/UnrealType.h"

#include "DataFactoryBPLibrary.generated.h"



UENUM(BlueprintType)
enum class EDataFactoryLogVerbosity : uint8
{
	/** Not used */
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
		VeryVerbose,

};

DECLARE_LOG_CATEGORY_EXTERN(DataFactoryLog, Log, All);

UENUM(BlueprintType)
enum class EInputBindingSupportedTypes : uint8
{
	ActionBinding,
	KeyBinding,
	AxisBinding
};


UCLASS()
class UDataFactoryBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()


		UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Key From Name", Keywords = "Get Key From Name"), Category = "ExposeRuntimeFunctions | Input")
		static FKey GetKeyFromName(FName name);



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

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Property Class Name"), Category = "DataFactory | General")
		static FString GetFPropertyClassName(UObject* Object, FName PropertyName);


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
	UFUNCTION(BlueprintCallable, Category = "DataFactory | Controller")
		static bool AddInputBinding(UObject* Object,
									FName SourceName,
									FName FunctionName,
									EInputBindingSupportedTypes InputBindingType,
									EInputEvent KeyEvent = IE_Released,
									UInputComponent* InputComponent = nullptr);

	static void BindActionInputInternal(UInputComponent* InputComponent, const FName& ActionName, UObject* Object, FName& FunctionName, EInputEvent KeyEvent);
	static void BindAxisInputInternal(UInputComponent* InputComponent, const FName& AxisName, UObject* Object, FName& FunctionName);
	static void BindKeyInputInternal(UInputComponent* InputComponent, const FName& KeyName, UObject* Object, FName& FunctionName, EInputEvent KeyEvent);


	UFUNCTION(BlueprintCallable, Category = "DataFactory | Import/ Export")
		static bool WriteStringToFile(const FString FileName, const FString DataToWrite);

	UFUNCTION(BlueprintGetter, Category = "DataFactory | Import/ Export")
		static bool ReadLinesFromFile(const FString FileName, TArray<FString>& LinesOut);



};
