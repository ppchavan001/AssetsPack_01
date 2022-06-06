// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeRuntimeFunctionsBPLibrary.h"
#include "ExposeRuntimeFunctions.h"
#include "UObject/PropertyAccessUtil.h"


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


void UExposeRuntimeFunctionsBPLibrary::SetFPropertyValueInternal(FProperty* property, UObject* Object, const FString DataToSet)
{
	// If property is valid for the object
	// Determine property type
	if (property && Object)
	{
		// *************************************************
		// 
		// FBool Property
		// 
		// *************************************************

		FBoolProperty* BoolProperty = CastField<FBoolProperty>(property);

		if (BoolProperty)
		{
			FString DataTrimmed = DataToSet.TrimStartAndEnd();

			bool bValue = DataTrimmed.Compare("true", ESearchCase::IgnoreCase) == 0;
			BoolProperty->SetPropertyValue(BoolProperty->ContainerPtrToValuePtr< void >(Object), bValue);

			return;
		}




		// *************************************************
		// 
		// FName Property
		// 
		// *************************************************


		FNameProperty* NameProperty = CastField<FNameProperty>(property);

		if (NameProperty)
		{
			FString DataTrimmed = DataToSet.TrimStartAndEnd();

			FName Value = FName(DataTrimmed);
			NameProperty->SetPropertyValue(NameProperty->ContainerPtrToValuePtr< void >(Object), Value);

			return;
		}



		// *************************************************
		// 
		// FString Property
		// 
		// *************************************************

		FStrProperty* StrProperty = CastField<FStrProperty>(property);
		if (StrProperty)
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
		// FText Property
		// 
		// *************************************************
		FTextProperty* TextProperty = CastField<FTextProperty>(property);
		if (TextProperty)
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
		// FNumeric Property 
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

			uint8 Data;
			StringToBytes(DataToSet, &Data, 1000);
			//ImplementPropertySetter(FByteProperty,		BytePropertyObject		, Data)
			ImplementPropertySetter(FDoubleProperty,	DoublePropertyObject	, FCString::Atod(*DataToSet))
			ImplementPropertySetter(FFloatProperty,		FloatPropertyObject		, FCString::Atof(*DataToSet))
			ImplementPropertySetter(FInt64Property,		Int64PropertyObject		, FCString::Atoi64(*DataToSet))
			ImplementPropertySetter(FUInt32Property,	UInt32PropertyObject	, FCString::Atoi(*DataToSet))
			ImplementPropertySetter(FInt16Property,		Int16PropertyObject		, FCString::Atoi(*DataToSet))
			ImplementPropertySetter(FInt8Property,		Int8PropertyObject		, FCString::Atoi(*DataToSet))
			ImplementPropertySetter(FIntProperty,		IntPropertyObject		, FCString::Atoi(*DataToSet))
			
		

			



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
				SetFPropertyValueInternal(*It, Object, ColorValue);
				UE_LOG(LogTemp, Warning, TEXT("Struct property : %s"), *(It->GetFName().ToString()));
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


