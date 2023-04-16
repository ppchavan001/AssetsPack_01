// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputCoreTypes.h"
#include "HapticsBindingActor.generated.h"


UCLASS()
class UNREALJSEXTENSIONS_API AHapticsBindingActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHapticsBindingActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealJSExtensions|HapticsBindingActor")
		UHapticFeedbackEffect_Base* _HapticEffect;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealJSExtensions|HapticsBindingActor")
	EControllerHand _hand;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealJSExtensions|HapticsBindingActor")
		float _Scale = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnrealJSExtensions|HapticsBindingActor")
		bool _bLoop = false;


	UFUNCTION(BlueprintCallable, Category = "UnrealJSExtensions|HapticsBindingActor")
		void SetupHapticEffect(
			UHapticFeedbackEffect_Base* HapticEffect,
			EControllerHand hand,
			float Scale = 1.0,
			bool bLoop = false);


	/* Usually bound to movement start input action. */
	UFUNCTION(BlueprintCallable, Category = "UnrealJSExtensions|HapticsBindingActor")
		void PlayHaptics(float Axis);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
