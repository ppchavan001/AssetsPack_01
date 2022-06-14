// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeRuntimeFunctionsBPLibrary.h"
#include "ExposeRuntimeFunctions.h"
#include "UObject/PropertyAccessUtil.h"
#include "UObject/UnrealType.h"

#include <string>

UExposeRuntimeFunctionsBPLibrary::UExposeRuntimeFunctionsBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FKey UExposeRuntimeFunctionsBPLibrary::GetKeyFromName(FName name)
{
	return FKey(name);

}

void UExposeRuntimeFunctionsBPLibrary::SetFPropertyByName(UObject* Object, FName NameOfThePropertyToUpdate, const FString DataToSet)
{

	if (Object)
	{
		UClass* _Class = Object->GetClass();

		FProperty* property = _Class->FindPropertyByName(NameOfThePropertyToUpdate);

		SetFPropertyValueInternal(property, Object, DataToSet);
	}
}


FString UExposeRuntimeFunctionsBPLibrary::BufferToString(const TArray<uint8>& DataBuffer)
{

	if (DataBuffer[DataBuffer.Num() - 1] == 0x00)
	{
		return UTF8_TO_TCHAR(DataBuffer.GetData());
	}

	TArray<uint8> tempBuffer;
	tempBuffer.SetNum(DataBuffer.Num() + 1);
	FMemory::Memcpy(tempBuffer.GetData(), DataBuffer.GetData(), DataBuffer.Num());
	tempBuffer[tempBuffer.Num() - 1] = 0x00;

	return UTF8_TO_TCHAR(tempBuffer.GetData());
}


void UExposeRuntimeFunctionsBPLibrary::StringToBuffer(const FString& message, TArray<uint8>& DataBuffer)
{
	std::string _str = TCHAR_TO_UTF8(*message);

	DataBuffer.SetNum(_str.size() + 1);
	DataBuffer[DataBuffer.Num() - 1] = 0x00;

	FMemory::Memcpy(DataBuffer.GetData(), _str.c_str(), _str.size());

}

#pragma optimize( "", off )

void UExposeRuntimeFunctionsBPLibrary::SetFPropertyValueInternal(FProperty* property, void* InContainer, const FString DataToSet)
{

#define Object InContainer

	// If property is valid for the object
	// Determine property type
	if (property && Object)
	{



		// *************************************************
		// 
		// Bool Property
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
		// Name Property
		// 
		// *************************************************

		if (FNameProperty* NameProperty = CastField<FNameProperty>(property))
		{
			FString DataTrimmed = DataToSet.TrimStartAndEnd();

			FName Value = FName(DataTrimmed);
			NameProperty->SetPropertyValue(NameProperty->ContainerPtrToValuePtr< void >(Object), Value);

			return;
		}



		// *************************************************
		// 
		// String Property
		// 
		// *************************************************

		if (FStrProperty* StrProperty = CastField<FStrProperty>(property))
		{
			FString StringData = DataToSet;


			// Remove unwanted chars outside of " " 
			StringData.Split("\"", NULL, &StringData);
			StringData.Split("\"", &StringData, NULL);


			StrProperty->SetPropertyValue(StrProperty->ContainerPtrToValuePtr< void >(Object), StringData);

			return;
		}





		// *************************************************
		// 
		// Text Property
		// 
		// *************************************************
		if (FTextProperty* TextProperty = CastField<FTextProperty>(property))
		{

			FString StringData = DataToSet;


			// Remove unwanted chars outside of " " 
			StringData.Split("\"", NULL, &StringData);
			StringData.Split("\"", &StringData, NULL);


			FText Value = FText::FromString(StringData);
			TextProperty->SetPropertyValue(TextProperty->ContainerPtrToValuePtr< void >(Object), Value);

			return;
		}



		// *************************************************
		// 
		// Enum Property
		// 
		// *************************************************
		if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(property))
		{
			FNumericProperty* UnderlyingProperty = EnumProperty->GetUnderlyingProperty();
			
			if (UnderlyingProperty)
			{
				SetFPropertyValueInternal(UnderlyingProperty, InContainer, DataToSet);
			}

			//e->a
			return;
		}

		// *************************************************
		// 
		// Array Property
		// 
		// Array of int, float, bool, etc
		// 
		// *************************************************
		if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(property))
		{

			// Build CSV array
			TArray<FString> DataArray;
			DataToSet.ParseIntoArray(DataArray, *FString(","), false);


			FScriptArrayHelper_InContainer Helper(ArrayProperty, InContainer);
		
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

				SetFPropertyValueInternal(ArrayProperty->Inner, ValuePtr, Data);

			}
		}



		// *************************************************
		// 
		// Numeric Property 
		// int, float, double
		// 
		// *************************************************
		FNumericProperty* NumericProperty = CastField<FNumericProperty>(property);

		if (NumericProperty)
		{


		#define ImplementPropertySetter(Type, PropertyObject, _Data)\
			{\
				Type* PropertyObject = CastField<Type>(NumericProperty);\
				if (PropertyObject)\
				{\
					PropertyObject->SetPropertyValue(PropertyObject->ContainerPtrToValuePtr< void >(Object), _Data);\
					return;\
				}\
			}\


		{

			FByteProperty* PropertyObject = CastField<FByteProperty>(NumericProperty);
			if (PropertyObject)
			{
				FString DataString = DataToSet.TrimStartAndEnd();
				int32 IntData  = FCString::Atoi(*DataString);

				PropertyObject->SetPropertyValue(PropertyObject->ContainerPtrToValuePtr< void >(Object), IntData);
				return;
			}
		}


		ImplementPropertySetter(FDoubleProperty, DoublePropertyObject, FCString::Atod(*DataToSet))
			ImplementPropertySetter(FFloatProperty, FloatPropertyObject, FCString::Atof(*DataToSet))
			ImplementPropertySetter(FInt64Property, Int64PropertyObject, FCString::Atoi64(*DataToSet))
			ImplementPropertySetter(FUInt32Property, UInt32PropertyObject, FCString::Atoi(*DataToSet))
			ImplementPropertySetter(FInt16Property, Int16PropertyObject, FCString::Atoi(*DataToSet))
			ImplementPropertySetter(FInt8Property, Int8PropertyObject, FCString::Atoi(*DataToSet))
			ImplementPropertySetter(FIntProperty, IntPropertyObject, FCString::Atoi(*DataToSet))







		}




		// *************************************************
		// 
		// FStruct Property 
		// vector, color, transform
		// 
		// *************************************************
		FStructProperty* StructProperty = CastField<FStructProperty>(property);
		if (StructProperty)
		{
			// Get Color data from string
			TArray<FString> ColorData;
			DataToSet.ParseIntoArray(ColorData, *FString(","), false);


			// Separate channel data by ":" from color data
			TMap<FString, FString> ChannelData;
			for (FString Data : ColorData)
			{
				FString Key;
				FString Value;

				Data.Split(":", &Key, &Value);

				Key.TrimStartAndEndInline();
				Value.TrimStartAndEndInline();


				ChannelData.Add(Key, Value);
			}



			// Update the data in the struct from ChannelData map
			for (TFieldIterator<FProperty> It(StructProperty->Struct); It; ++It)
			{

				FString CurrentChannelNameAsString = It->GetFName().ToString();
				const FString ColorValue = ChannelData[CurrentChannelNameAsString];
				volatile auto _a = *It;
				//UE_LOG(LogTemp, Warning, TEXT("Struct property : %s"), *(_a->NamePrivate.ToString()));
				//SetFPropertyValueInternal(*It, Object, ColorValue);
				//_a->setpro
				//const TCHAR* ch = &ColorValue[0];
				//It->ImportSingleProperty(ch, )

				//It->CopyCompleteValue_InContainer()


			}


			//FText Value = FText::FromString(DataToSet);
			//StructProperty->Struct->

			//	//SetPropertyValue(StructProperty->ContainerPtrToValuePtr< void >(Object), Value);
			//return;
		}

		//const EPropertyAccessResultFlags AccessResult = EPropertyAccessResultFlags::Success;//PropertyAccessUtil::SetPropertyValue_Object(ObjectProp, Object, ValueProp, ValuePtr, INDEX_NONE, PropertyAccessUtil::EditorReadOnlyFlags, ChangeNotifyMode);
		//if (AccessResult != EPropertyAccessResultFlags::Success)
		//{

		//	UE_LOG(LogTemp, Warning, TEXT("Hello"));
		//}



	}
}

#pragma optimize( "", on )
