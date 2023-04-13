// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_EnhancedInputManager.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <InputMappingContext.h>
#include <EnhancedPlayerInput.h>
#include <DataFactoryBPLibrary.h>
#include "UObject/UObjectGlobals.h"

void UBFL_EnhancedInputManager::BindActionOnEnhancedInputComponent(
	UEnhancedInputComponent* InputComponent,
	const UInputAction* Action,
	ETriggerEvent TriggerEvent,
	UObject* Object,
	FName FunctionName)
{
	InputComponent->BindAction(Action, TriggerEvent, Object, FunctionName);

}

void UBFL_EnhancedInputManager::AddNewInputMappingContext(const UObject* WorldContextObject, const UInputMappingContext* MappingContext, int32 Priority)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
				Subsystem->ClearAllMappings();

				// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
				Subsystem->AddMappingContext(MappingContext, 0);

			}
		}
	}
}

UObject* UBFL_EnhancedInputManager::NewUObjectOfClass(UObject* Outer, const UClass* Class)
{
	if (Class)
	{
		return NewObject<UObject>(Outer, Class);
	}
	return nullptr;
}

void UBFL_EnhancedInputManager::SetKeyOnInputAction(UInputAction* ia)
{ 

}





