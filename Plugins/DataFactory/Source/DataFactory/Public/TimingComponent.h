// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataFactoryBPLibrary.h"
#include "TimingComponent.generated.h"


USTRUCT(BlueprintType)
struct FLogParameters
{
	GENERATED_BODY()

	// if true Starts timing on construct
	// else will start on begin play
		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), AdvancedDisplay)
		bool bStartTimerOnConstruct = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), AdvancedDisplay)
		bool bPrintToLog = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), AdvancedDisplay)
		bool bPrintToScreen = true;

	// String to append before time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		FString Prefix = "";

	// String to append after time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), AdvancedDisplay)
		FString Postfix = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), AdvancedDisplay)
		EDataFactoryLogVerbosity LogVerbosity = EDataFactoryLogVerbosity::Log;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		FLinearColor OnScreenMessageColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		float TimeToDisplayMessageOnScreen = 2.0f;
};

/*
* Saves time at begin play and on destroyed.
* Prints delta time on destroyed.
*
* Should work properly for up to 24 hrs.
*
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		FLogParameters LogParameters;

public:
	// The time since the timer started
	UFUNCTION(BlueprintPure)
	const FTimespan GetTimeSpan();

private:
	FDateTime StartTime;

};
