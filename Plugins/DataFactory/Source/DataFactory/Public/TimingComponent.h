// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimingComponent.generated.h"


/*
* Saves time at begin play and on destroyed.
* Prints delta time on destroyed.
*
* Should work properly for up to 24 hrs.
*
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DATAFACTORY_API UTimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimingComponent();
	virtual void DestroyComponent(bool bPromoteChildren = false);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bStartTimerOnConstruct = true;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPrintToLog = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPrintToScreen = true;


private:
	FDateTime BeginPlayTime;
		
};
