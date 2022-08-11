// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataFactoryBPLibrary.h"
#include "DataFactory.h"


#include "UObject/PropertyAccessUtil.h"
#include "UObject/UnrealType.h"
#include "UObject/TextProperty.h"

#include <string>
#include <Misc/FileHelper.h>
#include "Components/InputComponent.h"
#include "Misc/CString.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/InputSettings.h"


DEFINE_LOG_CATEGORY(DataFactoryLog);

FString CurrentFileName = FString(__FILE__).RightChop(FString(__FILE__).Find(&FString("\\")[0], ESearchCase::IgnoreCase, ESearchDir::FromEnd) + 1);

UDataFactoryBPLibrary::UDataFactoryBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


void UDataFactoryBPLibrary::SetFPropertyByName(UObject* Object, FName NameOfThePropertyToUpdate, const FString DataToSet)
{

	if (Object)
	{
		UClass* _Class = Object->GetClass();

		FProperty* property = _Class->FindPropertyByName(NameOfThePropertyToUpdate);

		if (property)
		{
			SetFPropertyValueInternal(property, Object, DataToSet, NameOfThePropertyToUpdate);
		}
		else
		{
			return;
			// causing performance issues with logging with Everything!
			UE_LOG(DataFactoryLog, Warning, 
				   TEXT("DataFactoryBPLibrary.cpp : SetFPropertyByName : Couldn't find property name = %s."), 
				   *(NameOfThePropertyToUpdate.ToString()));
		}
	}
	else
	{
		UE_LOG(DataFactoryLog, Warning,
			   TEXT("DataFactoryBPLibrary.cpp : SetFPropertyByName : Invalid object provided."));
	}




}


FKey UDataFactoryBPLibrary::GetKeyFromName(FName name)
{
	return FKey(name);

}


void UDataFactoryBPLibrary::SetFPropertyValueInternal(FProperty* property, void* Object, const FString DataToSet, FName NameOfThePropertyToUpdate)
{
	// If property is valid for the object
	// Determine property type
	if (property && Object)
	{



		// *************************************************
		// 
		// BoolProperty		: valid values -> "true" or "false"
		// 
		// *************************************************

		if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(property))
		{
			FString DataTrimmed = DataToSet.TrimStartAndEnd();

			bool bValue = DataTrimmed.Compare("true", ESearchCase::IgnoreCase) == 0;
			BoolProperty->SetPropertyValue(BoolProperty->ContainerPtrToValuePtr< void >(Object), bValue);

			return;
		}



		// *************************************************
		// 
		// NameProperty	: valid values ->  any string will be converted to name
		// 
		// *************************************************

		if (FNameProperty* NameProperty = CastField<FNameProperty>(property))
		{
			FString DataTrimmed = DataToSet.TrimStartAndEnd();

			FName Value = FName(*DataTrimmed);
			NameProperty->SetPropertyValue(NameProperty->ContainerPtrToValuePtr< void >(Object), Value);

			return;
		}



		// *************************************************
		// 
		// StrProperty	: valid values ->  any string
		// 
		// *************************************************

		if (FStrProperty* StrProperty = CastField<FStrProperty>(property))
		{
			StrProperty->SetPropertyValue(StrProperty->ContainerPtrToValuePtr< void >(Object), DataToSet);

			return;
		}





		// *************************************************
		// 
		// TextProperty		: valid values -> Any string will be converted to text
		// 
		// *************************************************
		if (FTextProperty* TextProperty = CastField<FTextProperty>(property))
		{
			FText Value = FText::FromString(DataToSet);
			TextProperty->SetPropertyValue(TextProperty->ContainerPtrToValuePtr< void >(Object), Value);

			return;
		}




		// *************************************************
		// 
		// EnumProperty		: valid values -> 0 to (255 or max Enum size which ever is lower), 
		// 
		// *************************************************
		if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(property))
		{
			FNumericProperty* UnderlyingProperty = EnumProperty->GetUnderlyingProperty();

			if (UnderlyingProperty)
			{
				auto PropertyValuePtr = (EnumProperty->ContainerPtrToValuePtr< void >(Object));
				FString DataString = DataToSet.TrimStartAndEnd();

				if (DataString.Len() == 0) DataString = "0";

				UnderlyingProperty->SetNumericPropertyValueFromString(PropertyValuePtr, &(DataString[0]));

			}

			return;
		}




		// *************************************************
		// 
		// Array Property
		// 
		// Array of int, float, bool, etc
		// : valid values -> Comma separated values of the supported type
		// 
		// *************************************************
		if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(property))
		{

			// Build CSV array
			TArray<FString> DataArray;
			DataToSet.ParseIntoArray(DataArray, *FString(","), false);


			FScriptArrayHelper_InContainer Helper(ArrayProperty, Object);

			// Clear old values
			Helper.EmptyValues(DataArray.Num());

			// Add empty values for assignment
			Helper.AddValues(DataArray.Num());



			// Assign string values from Data Array to Array property
			volatile auto x = Helper.Num();

			for (int32 DynamicIndex = 0; DynamicIndex < Helper.Num(); ++DynamicIndex)
			{
				void* ValuePtr = Helper.GetRawPtr(DynamicIndex);


				// Data Cleaning and validation
				FString Data = DataArray[DynamicIndex];
				Data.TrimStartAndEndInline();
				if (Data.Len() < 1)	Data = "0";

				SetFPropertyValueInternal(ArrayProperty->Inner, ValuePtr, Data, NameOfThePropertyToUpdate);

			}

			return;
		}



		// *************************************************
		// 
		// Numeric Property 
		// int, float, double
		// 
		// *************************************************

		if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(property))
		{
			auto PropertyValuePtr = (NumericProperty->ContainerPtrToValuePtr< void >(Object));
			FString DataString = DataToSet.TrimStartAndEnd();

			if (DataString.Len() == 0) DataString = "0";
			NumericProperty->SetNumericPropertyValueFromString(PropertyValuePtr, &(DataString[0]));

			return;
		}




		// *************************************************
		// 
		// FStruct Property 
		// vector, color, transform
		// : 
		//	Color					: comma separated keys ex. R:0, G: 25, B:127, A:255 or CSV in same order
		//	Vector(location, scale)	: comma separated keys ex. LocX: 125, LocY: 778, LocZ : -220 or CSV in same order
		//	Rotator					: comma separated keys ex. RotX : -75, RotY: 76, RotZ: -650 or CSV in same order
		//	Transform				: comma separated keys ex. LocX: 400,  RotZ: -100,   ScaleX: 12.2, ScaleY : -54, ScaleZ: 70 
		//								or CSV in Loc(x,y,z), Rotation(x,y,z), Scale(x,y,z)
		//
		// 
		// *************************************************


		FStructProperty* StructProperty = CastField<FStructProperty>(property);
		if (StructProperty)
		{
			// ---------------------
			// Setup Start
			// ---------------------

			FString StructTypeName = FString(StructProperty->Struct->GetName());


			// Get CSV data from string
			TArray<FString> CSVArray;
			DataToSet.ParseIntoArray(CSVArray, *FString(","), false);


			// Separate channel data by ":" from CSV data
			TMap<FString, FString> ChannelData;
			for (FString Data : CSVArray)
			{
				FString Key;
				FString Value;

				Data.Split(":", &Key, &Value);

				Key.TrimStartAndEndInline();
				Value.TrimStartAndEndInline();


				// If key is present then add it as a channel
				if (Key.Len() > 0)	ChannelData.Add(Key, Value);

			}



		/*-----------------------------------------------------------
		|															|
		|	Key Definitions	to lookup in loaded strings				|
		|															|
		-------------------------------------------------------------*/



		// Update this value defending on the default value of the channel 
		// ex Location = "0", scale = "1"
		FString DefaultChannelValue = "This value should never be used! Replace with default value for the channel";

		// Return FString for specified channel
		// If key is defined 
		//		return value for key
		// else 
		//	if index is valid
		//		return Value at index 
		//	else 
		//		return default value for channel
		/* Equivalent to 
		auto AssignChannelValue = [](FString Key, int32 Index, TMap<FString, FString> ChannelData, TArray<FString> CSVArray)
		{

			if (ChannelData.Contains(Key) && ChannelData[Key].Len() > 0)
			{
				return ChannelData[Key];
			}

			if (CSVArray.Num() > Index && !(CSVArray[Index].Contains(":")))
			{
				return CSVArray[Index];
			}

			return FString("0"); // DefaultChannelValue;


		};*/
		#define AssignChannelValue(Key, Index) (ChannelData.Contains(Key) && ChannelData[Key].Len() > 0) ? \
				 /* If contains valid key, return immediately */ ChannelData[Key] : \
				/* if key is not present, if array contains valid value, return array val otherwise return default channel val. */ \
				(CSVArray.Num() > Index && CSVArray[Index].Len() > 0 &&!(CSVArray[Index].Contains(":"))) ?  CSVArray[Index] : DefaultChannelValue


		#define ColorR	"R"
		#define ColorG 	"G"
		#define ColorB 	"B"
		#define ColorA 	"A"

		#define LocationX "LocX"
		#define LocationY "LocY"
		#define LocationZ "LocZ"


		#define RotationX "RotX"
		#define RotationY "RotY"
		#define RotationZ "RotZ"
		#define RotationW "RotW"

		#define ScaleX "ScaleX"
		#define ScaleY "ScaleY"
		#define ScaleZ "ScaleZ"

		#define ImplementLocation(LocationXIndex, LocationYIndex, LocationZIndex) \
			FVector Location;\
			Location.X = FCString::Atof(&(AssignChannelValue(LocationX, LocationXIndex))[0]);\
			Location.Y = FCString::Atof(&(AssignChannelValue(LocationY, LocationYIndex))[0]);\
			Location.Z = FCString::Atof(&(AssignChannelValue(LocationZ, LocationZIndex))[0]);

		#define ImplementRotator(RotationXIndex, RotationYIndex, RotationZIndex, RotationWIndex) \
			FQuat Rotation;\
			Rotation.X = FCString::Atof(&(AssignChannelValue(RotationX, RotationXIndex))[0]);\
			Rotation.Y = FCString::Atof(&(AssignChannelValue(RotationY, RotationYIndex))[0]);\
			Rotation.Z = FCString::Atof(&(AssignChannelValue(RotationZ, RotationZIndex))[0]);\
			Rotation.W = FCString::Atof(&(AssignChannelValue(RotationW, RotationWIndex))[0]);


		//Reset warning for macros with multiple values
		#pragma warning (default: 4003) 

			// ---------------------
			// Setup End
			// ---------------------


			// Vector i.e location, scale, etc
			if (StructTypeName == "Vector")
			{
				DefaultChannelValue = "0";

				ImplementLocation(0, 1, 2);

				StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr< void >(Object), &Location);

				return;

			}



			// Rotation
			if (StructTypeName == "Rotator")
			{
				DefaultChannelValue = "0";

				ImplementRotator(0, 1, 2, 3);

				StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr< void >(Object), &Rotation);

				return;

			}


			// Color Struct
			// 
			// UInt8 for each channel
			// hence limited to (0-255) range
			//
			
			if (StructTypeName == "Color")
			{

				DefaultChannelValue = "255";


				FColor ColorValue;
				ColorValue.R = FCString::Atoi(&(AssignChannelValue(ColorR, 0))[0]);
				ColorValue.G = FCString::Atoi(&(AssignChannelValue(ColorG, 1))[0]);
				ColorValue.B = FCString::Atoi(&(AssignChannelValue(ColorB, 2))[0]);
				ColorValue.A = FCString::Atoi(&(AssignChannelValue(ColorA, 3))[0]);

				StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr< void >(Object), &ColorValue);

				return;

			}

			// Transform
			if (StructTypeName == "Transform")
			{

				DefaultChannelValue = "0";

				ImplementLocation(0, 1, 2);
				ImplementRotator(3, 4, 5, 9);



				DefaultChannelValue = "1";

				FVector Scale;
				Scale.X = FCString::Atof(&(AssignChannelValue(ScaleX, 6))[0]);
				Scale.Y = FCString::Atof(&(AssignChannelValue(ScaleY, 7))[0]);
				Scale.Z = FCString::Atof(&(AssignChannelValue(ScaleZ, 8))[0]);



				FTransform Transform;
				Transform.SetComponents(Rotation, Location, Scale);

				StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr< void >(Object), &Transform);

				return;
			}



			//
			// Linear Color Struct
			// 
			// Channels defined by float variables
			// hence can store any value in floating point range
			// 
			//
			if (StructTypeName == "LinearColor")
			{

				DefaultChannelValue = "1";


				FLinearColor ColorValue;
				ColorValue.R = FCString::Atof(&(AssignChannelValue(ColorR, 0))[0]);
				ColorValue.G = FCString::Atof(&(AssignChannelValue(ColorG, 1))[0]);
				ColorValue.B = FCString::Atof(&(AssignChannelValue(ColorB, 2))[0]);
				ColorValue.A = FCString::Atof(&(AssignChannelValue(ColorA, 3))[0]);

				StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr< void >(Object), &ColorValue);

				return;

			}


			UE_LOG(DataFactoryLog, Warning, 
				   TEXT("DataFactoryBPLibrary.cpp : SetFPropertyValueInternal : Tried to set value of unsupported struct type = %s , Property name = %s"), 
				   *StructTypeName, *(NameOfThePropertyToUpdate.ToString()));

			return;
		}

		UE_LOG(DataFactoryLog, Warning, TEXT("DataFactoryBPLibrary.cpp : SetFPropertyValueInternal : Tried to set value of unsupported type. Property Name = %s"), *(NameOfThePropertyToUpdate.ToString()));

		
	}

	
	UE_LOG(DataFactoryLog, Warning, 
		   TEXT("DataFactoryBPLibrary.cpp : SetFPropertyValueInternal : The property or the object provided is invalid. DataToSet = %s"),
		   *(DataToSet));

}


FString UDataFactoryBPLibrary::GetFPropertyClassName(UObject* Object, FName PropertyName)
{
	if (Object)
	{
		UClass* _Class = Object->GetClass();

		FProperty* property = _Class->FindPropertyByName(PropertyName);

		if (property) return property->GetClass()->GetName();

		return FString("Invalid Property name!");

	}
	return FString("Invalid Object!");
}

#pragma optimize("", on)
bool UDataFactoryBPLibrary::AddInputBinding(UObject* Object,
											FName SourceName,
											FName FunctionName,
											EInputBindingSupportedTypes InputBindingType,
											EInputEvent KeyEvent,
											UInputComponent* InputComponent)
{

	if (Object && Object->GetWorld() && Object->GetWorld()->GetFirstPlayerController())
	{
		// Required object checks start **************************
		UInputSettings* InputSettings = UInputSettings::GetInputSettings();
		if (!InputSettings)
		{
			UE_LOG(DataFactoryLog, Error, TEXT("%s::%s::%d : Couldn't find Input settings for Action : %s!"),
				   *CurrentFileName, *FString(__func__), __LINE__, *(SourceName.ToString()));

			return false;
		}

		// Proceed only if a function with the FunctionName exists on the provided Object
		if (!(Object->GetClass()->FindFunctionByName(FunctionName)))
		{
			UE_LOG(DataFactoryLog, Error, TEXT("%s::%s::%d : Object doesn't have the specified function : %s, for binding : %s!"),
				   *CurrentFileName, *FString(__func__), __LINE__, *(FunctionName.ToString()) , *(SourceName.ToString()));

			return false;
		}


		// If provided action/ axis name is not present in action mappings at runtime:
		// Return false
		bool ActionPresentInSettings = false;
		switch (InputBindingType)
		{
		case EInputBindingSupportedTypes::ActionBinding:
			ActionPresentInSettings = InputSettings->DoesActionExist(SourceName);
			break;

		case EInputBindingSupportedTypes::KeyBinding:
			ActionPresentInSettings = true;
			break;

		case EInputBindingSupportedTypes::AxisBinding:
			ActionPresentInSettings = InputSettings->DoesAxisExist(SourceName);
			break;
		}

		if (!ActionPresentInSettings)
		{

			UE_LOG(DataFactoryLog, Error, TEXT("%s::%s::%d : Action name : %s, not present in InputSettings!"),
				   *CurrentFileName, *FString(__func__), __LINE__, *(SourceName.ToString()));

			return false;
		}

		// Required object checks end **************************


		// Input component setup start ************************
		// if no input component is provided, look for one
		if (!InputComponent)
		{
			auto Player0Controller = Object->GetWorld()->GetFirstPlayerController();

			if (Player0Controller->InputComponent)
			{
				InputComponent = Player0Controller->InputComponent;
			}
			else
			{
				UE_LOG(DataFactoryLog, Error, TEXT("%s::%s::%d : InputComponent not found on the player0! Returning false for function: %s, action : %s"),
					   *CurrentFileName, *FString(__func__), __LINE__, *(FunctionName.ToString()), *(SourceName.ToString()));

				return false;
			}
		}

		// Input component setup End ************************


		switch (InputBindingType)
		{
		case EInputBindingSupportedTypes::ActionBinding:
			BindActionInputInternal(InputComponent, SourceName, Object, FunctionName, KeyEvent);
			break;
	
		case EInputBindingSupportedTypes::KeyBinding:
			BindKeyInputInternal(InputComponent, SourceName, Object, FunctionName, KeyEvent);
			break;
		
		case EInputBindingSupportedTypes::AxisBinding:
			BindAxisInputInternal(InputComponent, SourceName, Object, FunctionName);
			break;
		}

		return true;

	}

	UE_LOG(DataFactoryLog, Error, TEXT("%s::%s::%d : Invalid actor or world or player0 controller!"),
		   *CurrentFileName, *FString(__func__), __LINE__);
	return false;
}

void UDataFactoryBPLibrary::BindActionInputInternal(UInputComponent* InputComponent, 
													const FName& ActionName, 
													UObject* Object, 
													FName& FunctionName, 
													EInputEvent KeyEvent)
{

	// Searching for action on the InputComponent
	int32 ActionIndex = -1;
	for (int32 i = 0; i < InputComponent->GetNumActionBindings(); ++i)
	{
		if (InputComponent->GetActionBinding(i).GetActionName() == ActionName &&
			InputComponent->GetActionBinding(i).KeyEvent == KeyEvent)
		{
			ActionIndex = i;
			break;
		}
	}


	if (ActionIndex > -1) 
		InputComponent->GetActionBinding(ActionIndex).ActionDelegate.GetDelegateForManualSet().BindUFunction(Object, FunctionName);
		
	else
	{
		// Action binding not found on the controller,
		// Create and assign new action binding

		FInputActionBinding NewActionBinding(ActionName, KeyEvent);
		NewActionBinding.ActionDelegate.GetDelegateForManualSet().BindUFunction(Object, FunctionName);
		InputComponent->AddActionBinding(NewActionBinding);

	}


	UE_LOG(DataFactoryLog, Log, TEXT("%s::%s::%d : Bound function : %s to ActionMapping: %s "),
		   *CurrentFileName, *FString(__func__), __LINE__, *(FunctionName.ToString()), *(ActionName.ToString()));

}


void UDataFactoryBPLibrary::BindAxisInputInternal(UInputComponent* InputComponent, 
												  const FName& AxisName,
												  UObject* Object, 
												  FName& FunctionName)
{

	// Searching for action on the InputComponent
	int32 AxisIndex = -1;
	for (int32 i = 0; i < InputComponent->AxisBindings.Num(); ++i)
	{
		if (InputComponent->AxisBindings[i].AxisName == AxisName)
		{
			AxisIndex = i;
			break;
		}
	}


	if (AxisIndex > -1)
		InputComponent->AxisBindings[AxisIndex].AxisDelegate.GetDelegateForManualSet().BindUFunction(Object, FunctionName);

	else
	{
		// Key binding not found on the controller,
		// Create and assign new Key binding
		FInputAxisBinding NewAxisBinding(AxisName);
		NewAxisBinding.AxisDelegate.GetDelegateForManualSet().BindUFunction(Object, FunctionName);
		InputComponent->AxisBindings.Add(NewAxisBinding);
	}



	UE_LOG(DataFactoryLog, Log, TEXT("%s::%s::%d : Bound function : %s to AxisMapping: %s "),
		   *CurrentFileName, *FString(__func__), __LINE__, *(FunctionName.ToString()), *(AxisName.ToString()));
}


void UDataFactoryBPLibrary::BindKeyInputInternal(UInputComponent* InputComponent, const FName& KeyName, UObject* Object, FName& FunctionName, EInputEvent KeyEvent)
{

	// Searching for action on the InputComponent
	int32 KeyIndex = -1;
	for (int32 i = 0; i < InputComponent->KeyBindings.Num(); ++i)
	{
		if (InputComponent->KeyBindings[i].Chord.GetKeyText().ToString() == KeyName.ToString() && 
			InputComponent->KeyBindings[i].KeyEvent == KeyEvent)
		{
			KeyIndex = i;
			break;
		}
	}


	if (KeyIndex > -1)
		InputComponent->KeyBindings[KeyIndex].KeyDelegate.GetDelegateForManualSet().BindUFunction(Object, FunctionName);

	else
	{
		// Key binding not found on the controller,
		// Create and assign new Key binding

		FInputKeyBinding NewKeyBinding(FInputChord(FKey(KeyName)), KeyEvent);
		NewKeyBinding.KeyDelegate.GetDelegateForManualSet().BindUFunction(Object, FunctionName);
		InputComponent->KeyBindings.Add(NewKeyBinding);
	}


	UE_LOG(DataFactoryLog, Log, TEXT("%s::%s::%d : Bound function : %s to Key: %s "),
		   *CurrentFileName, *FString(__func__), __LINE__, *(FunctionName.ToString()), *(KeyName.ToString()));
}

#pragma optimize("", on)

bool UDataFactoryBPLibrary::WriteStringToFile(const FString FileName, const FString DataToWrite)
{
	return FFileHelper::SaveStringToFile(DataToWrite, &FileName[0]);
}
bool UDataFactoryBPLibrary::ReadLinesFromFile(const FString FileName, TArray<FString>& LinesOut)
{
	return FFileHelper::LoadFileToStringArray(LinesOut, &FileName[0]);
}

