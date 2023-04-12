// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedInputManager_Backend.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <InputMappingContext.h>
#include <EnhancedPlayerInput.h>


// Sets default values
AEnhancedInputManager_Backend::AEnhancedInputManager_Backend()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnhancedInputManager_Backend::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnhancedInputManager_Backend::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UInputMappingContext* AEnhancedInputManager_Backend::GetCurrentMappingContext(const UObject* WorldContextObject)
{
	return AllAvailableInputContexts[0];
}

