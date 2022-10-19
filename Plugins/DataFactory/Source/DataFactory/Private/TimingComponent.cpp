// Fill out your copyright notice in the Description page of Project Settings.


#include "TimingComponent.h"
#include "Engine/Console.h"
#include "Logging/LogVerbosity.h"

// Sets default values for this component's properties
UTimingComponent::UTimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	if (LogParameters.bStartTimerOnConstruct) ResumeTimerPrivate(true);
	// ...
}


void UTimingComponent::DestroyComponent(bool bPromoteChildren /*= false*/)
{
	FTimespan DeltaTime = GetTimeSpan();

	FString FinalDisplayString = LogParameters.Prefix;
	FinalDisplayString += DeltaTime.ToString();
	FinalDisplayString += LogParameters.Postfix;

	UDataFactoryBPLibrary::DF_PrintString(this, FinalDisplayString,
										  LogParameters.LogVerbosity,
										  LogParameters.bPrintToScreen,
										  LogParameters.bPrintToScreen,
										  LogParameters.TimeToDisplayMessageOnScreen);
}

// Called when the game starts
void UTimingComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!LogParameters.bStartTimerOnConstruct) ResumeTimerPrivate(true);

}

const FTimespan UTimingComponent::GetTimeSpan()
{
	PauseTimer();
	FTimespan Timespan = FTimespan(0);

	for (auto Timings : TimeSpanData)
	{
		Timespan += (Timings[1] - Timings[0]);
	}


	ResumeTimer();
	return Timespan;
}

void UTimingComponent::PauseTimer()
{
	// only pause if not already paused
	if (bIsPaused) return;
	
	TArray<FDateTime> temp;
	temp.Add(LastStartTime);
	temp.Add(FDateTime::Now());

	TimeSpanData.Add(temp);
	bIsPaused = true;
}

void UTimingComponent::ResumeTimerPrivate(bool bForceResume)
{
	// only resume if paused
	if (bIsPaused || bForceResume)
	{
		LastStartTime = FDateTime::Now();
		bIsPaused = false;
	}
}

