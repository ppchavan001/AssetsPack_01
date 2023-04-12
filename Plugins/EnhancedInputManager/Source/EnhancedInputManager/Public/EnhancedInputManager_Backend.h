// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnhancedInputManager_Backend.generated.h"

class UInputMappingContext;

UCLASS()
class ENHANCEDINPUTMANAGER_API AEnhancedInputManager_Backend : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnhancedInputManager_Backend();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<UInputMappingContext*> AllAvailableInputContexts;


	UFUNCTION(BlueprintCallable, Category = "EnhancedInputManager|EnhancedInput")
		UInputMappingContext* GetCurrentMappingContext(const UObject* WorldContextObject);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
