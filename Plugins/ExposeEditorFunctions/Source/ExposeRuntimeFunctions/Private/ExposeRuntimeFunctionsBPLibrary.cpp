// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeRuntimeFunctionsBPLibrary.h"
#include "ExposeRuntimeFunctions.h"

UExposeRuntimeFunctionsBPLibrary::UExposeRuntimeFunctionsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FKey UExposeRuntimeFunctionsBPLibrary::ExposeRuntimeFunctionsSampleFunction(FName name)
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

		   /*Cast<FArrayProperty*>(property)
		   Cast<FEnumProperty*>(property)
		   Cast<FMapProperty*>(property)
		   Cast<FNameProperty*>(property)
		   Cast<FNumericProperty*>(property)
		   Cast<FSetProperty*>(property)
		   Cast<FStrProperty*>(property)
		   Cast<FStructProperty*>(property)
		   Cast<FTextProperty*>(property)*/

		}
	}
}
