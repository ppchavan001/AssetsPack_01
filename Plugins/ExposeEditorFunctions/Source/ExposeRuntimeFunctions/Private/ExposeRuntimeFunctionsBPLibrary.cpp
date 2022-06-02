// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeRuntimeFunctionsBPLibrary.h"
#include "ExposeRuntimeFunctions.h"

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
		
		if (property)
		{

			FBoolProperty* BoolProperty = CastFieldChecked<FBoolProperty>(property);

			if (BoolProperty)
			{
				bool bValue = DataToSet.Compare("true", ESearchCase::IgnoreCase) == 0;
				BoolProperty->SetPropertyValue(BoolProperty->ContainerPtrToValuePtr< void >(Object), bValue);

				return;
			}

			FNameProperty* NameProperty = CastFieldChecked<FNameProperty>(property);

			if (NameProperty)
			{
				FName Value = FName(DataToSet);
				NameProperty->SetPropertyValue(NameProperty->ContainerPtrToValuePtr< void >(Object), Value);

				return;
			}

			FStrProperty* StrProperty = CastFieldChecked<FStrProperty>(property);
			if (StrProperty)
			{
				StrProperty->SetPropertyValue(StrProperty->ContainerPtrToValuePtr< void >(Object), DataToSet);

				return;
			}


			FTextProperty* TextProperty = CastFieldChecked<FTextProperty>(property);
			if (TextProperty)
			{
				FText Value = FText::FromString(DataToSet);
				TextProperty->SetPropertyValue(TextProperty->ContainerPtrToValuePtr< void >(Object), Value);

				return;
			}


			FNumericProperty* NumericProperty = CastFieldChecked<FNumericProperty>(property);

			if (NumericProperty)
			{



				FFloatProperty* FloatProperty = CastFieldChecked<FFloatProperty			>(NumericProperty);

				if (FloatProperty)
				{
					FloatProperty->SetPropertyValue(FloatProperty->ContainerPtrToValuePtr< void >(Object), FCString::Atof(*DataToSet));
					return;
				}

				FInt64Property* Int64Property = CastFieldChecked<FInt64Property			>(NumericProperty);

				if (Int64Property)
				{
					Int64Property->SetPropertyValue(Int64Property->ContainerPtrToValuePtr< void >(Object),
													FCString::Strtoui64(*DataToSet, nullptr, 10));
					return;
				}


				FIntProperty* IntProperty = CastFieldChecked<FIntProperty			>(NumericProperty);

				if (IntProperty)
				{
					IntProperty->SetPropertyValue(IntProperty->ContainerPtrToValuePtr< void >(Object),
													FCString::Atoi(*DataToSet));
					return;
				}




			}

				/*
		   CastFieldChecked<FEnumProperty>(property)
		   CastFieldChecked<FArrayProperty>(property)
		   CastFieldChecked<FSetProperty>(property)
		   CastFieldChecked<FStructProperty>(property)
		   CastFieldChecked<FMapProperty>(property)
		   */

		}
	}
}
