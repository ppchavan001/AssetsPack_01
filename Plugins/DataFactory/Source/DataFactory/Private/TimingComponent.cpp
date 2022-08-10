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
	if (bStartTimerOnConstruct) BeginPlayTime = FDateTime::Now();
	// ...
}


void UTimingComponent::DestroyComponent(bool bPromoteChildren /*= false*/)
{
	FTimespan DeltaTime = FDateTime::Now() - BeginPlayTime;

	FString FinalDisplayString = Prefix;
	FinalDisplayString += DeltaTime.ToString();
	FinalDisplayString += Postfix;

	if (bPrintToLog)
	{
		GLog->Log(DataFactoryLog.GetCategoryName(), (ELogVerbosity::Type)(LogVerbosity), &FinalDisplayString[0]);
	}

	if (this->GetWorld() && this->GetWorld()->GetFirstPlayerController())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplayMessageOnScreen, OnScreenMessageColor.ToFColor(true), FinalDisplayString);
	}
}

// Called when the game starts
void UTimingComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!bStartTimerOnConstruct) BeginPlayTime = FDateTime::Now();

}


// Called every frame
void UTimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

