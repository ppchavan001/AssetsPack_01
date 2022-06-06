// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeRuntimeFunctionsBPLibrary.h"
#include "ExposeRuntimeFunctions.h"
#include "UObject/PropertyAccessUtil.h"



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

		SetFPropertyValue(property, Object, DataToSet);
	}
}


void UExposeRuntimeFunctionsBPLibrary::SetFPropertyValue(FProperty* property, UObject* Object, const FString DataToSet)
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

			FFloatProperty* FloatProperty = CastField<FFloatProperty>(NumericProperty);

			if (FloatProperty)
			{
				FloatProperty->SetPropertyValue(FloatProperty->ContainerPtrToValuePtr< void >(Object), FCString::Atof(*DataToSet));
				return;
			}

			FInt64Property* Int64Property = CastField<FInt64Property>(NumericProperty);

			if (Int64Property)
			{
				Int64Property->SetPropertyValue(Int64Property->ContainerPtrToValuePtr< void >(Object),
												FCString::Strtoui64(*DataToSet, nullptr, 10));
				return;
			}


			FIntProperty* IntProperty = CastField<FIntProperty>(NumericProperty);

			if (IntProperty)
			{
				IntProperty->SetPropertyValue(IntProperty->ContainerPtrToValuePtr< void >(Object),
											  FCString::Atoi(*DataToSet));
				return;
			}




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
				SetFPropertyValue(*It, Object, ChannelData[It->GetFName().ToString()]);
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

			/*
	   CastField<FEnumProperty>(property)
	   CastField<FArrayProperty>(property)
	   CastField<FSetProperty>(property)
	   CastField<FStructProperty>(property)
	   CastField<FMapProperty>(property)
	   */

	}
}

