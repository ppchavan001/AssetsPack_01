// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_EnhancedInputManager.generated.h"

class UEnhancedInputComponent;
class UInputAction;
class UInputMappingContext;
/**
 *
 */
UCLASS()
class ENHANCEDINPUTMANAGER_API UBFL_EnhancedInputManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable, Category = "EnhancedInputManager|InputAction")
		static void BindActionOnEnhancedInputComponent(
			UEnhancedInputComponent* InputComponent,
			const UInputAction* Action,
			ETriggerEvent TriggerEvent,
			UObject* Object,
			FName FunctionName);

	UFUNCTION(BlueprintCallable, Category = "EnhancedInputManager|MappingContext")
		static void AddNewInputMappingContext(const UObject* WorldContextObject, const UInputMappingContext* MappingContext, int32 Priority);


	UFUNCTION(BlueprintCallable, Category = "EnhancedInputManager|Memory")
	static UObject* NewUObjectOfClass(UObject* Outer, const UClass* Class);

	UFUNCTION(BlueprintCallable, Category = "EnhancedInputManager|Memory")
		static void SetKeyOnInputAction(UInputAction* ia);

};
