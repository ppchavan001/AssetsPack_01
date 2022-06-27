// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputCore/Classes/InputCoreTypes.h"
#include "UObject/UnrealType.h"

#include "ExposeRuntimeFunctionsBPLibrary.generated.h"
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

UCLASS()
class UExposeRuntimeFunctionsBPLibrary : public UBlueprintFunctionLibrary
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
		EnumProperty		: valid values -> 0 to (255 or max Enum size which ever is lower), 
		ArrayProperty		: array of all supported types : valid values -> Comma separated values of the supported type, 
		NumericProperty		: all numeric types : ex float, int, double, byte
		StructProperty		: 
					//	Color					: comma separated keys ex. R:0, G: 25, B:127, A:255 or CSV in same order
					//	Vector(location, scale)	: comma separated keys ex. LocX: 125, LocY: 778, LocZ : -220 or CSV in same order
					//	Rotator					: comma separated keys ex. RotX : -75, RotY: 76, RotZ: -650 or CSV in same order
					//	Transform				: comma separated keys ex. LocX: 400,  RotZ: -100,   ScaleX: 12.2, ScaleY : -54, ScaleZ: 70 
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
	UFUNCTION(BlueprintCallable, Category = "ExposeRuntimeFunctions | General", meta = (DisplayName = "Set Property Value By Name"))
	static void SetFPropertyByName(UObject* Object, FName NameOfThePropertyToUpdate, const FString DataToSet);

	static void SetFPropertyValueInternal(FProperty* property, void* Object, const FString DataToSet);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Property Class Name"), Category = "ExposeRuntimeFunctions | General")
	static FString GetFPropertyClassName(UObject* Object, FName PropertyName);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Buffer To String"), Category = "ExposeRuntimeFunctions | Type Conversions")
	static FString BufferToString(const TArray<uint8>& DataBuffer);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "String To Buffer"), Category = "ExposeRuntimeFunctions | Type Conversions")
	static void StringToBuffer(const FString& message, TArray<uint8>& DataBuffer);

	/*
	* String Input:
		X=-0.000 Y=21.332 Z=-0.476
		X=-0.000 Y=22.324 Z=-0.476
		X=-0.000 Y=23.316 Z=-0.476
		

		Output 
		Array of vectors with 3 elements

		Note: Will clear VerticesOut before filling it with vectors
	*/
	UFUNCTION(BlueprintCallable)
	void ConvertStringToVector(TArray<FString> Lines, TArray<FVector>& VerticesOut);
};
