// Fill out your copyright notice in the Description page of Project Settings.


#include "HapticsBindingActor.h"

// Sets default values
AHapticsBindingActor::AHapticsBindingActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AHapticsBindingActor::SetupHapticEffect(UHapticFeedbackEffect_Base* HapticEffect, EControllerHand hand, float Scale, bool bLoop)
{
	_HapticEffect = HapticEffect;
	_hand = hand;
	_Scale = Scale;
	_bLoop = bLoop;
}

void AHapticsBindingActor::PlayHaptics(float Axis)
{
	if (auto world = GetWorld())
	{
		if (auto pc = world->GetFirstPlayerController())
		{
			pc->PlayHapticEffect(_HapticEffect, _hand, _Scale, _bLoop);

			//UE_LOG(LogTemp, Log, TEXT("Playing haptics on  %d"), int(_hand));
		}
	}
}

// Called when the game starts or when spawned
void AHapticsBindingActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHapticsBindingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

