// Fill out your copyright notice in the Description page of Project Settings.


#include "TraceActorSource.h"
#include "EngineUtils.h"
#include "Engine/World.h"

#include "DataFactoryBPLibrary.h"

// Sets default values
ATraceActorSource::ATraceActorSource()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetTickGroup(ETickingGroup::TG_PrePhysics);
}

// Called when the game starts or when spawned
void ATraceActorSource::BeginPlay()
{
	Super::BeginPlay();
	UpdateComponentToTraceByName();

}

/*
bool ATraceActorSource::UpdateTrace(TArray<struct FHitResult>& OutHits,
									const FVector& Start,
									const FVector& End,
									const FCollisionObjectQueryParams& ObjectQueryParams,
									const FCollisionQueryParams& Params) const
{
	if (this->GetWorld())
		return this->GetWorld()->LineTraceMultiByObjectType(OutHits, Start, End, ObjectQueryParams, Params);

	return false;
}*/

bool ATraceActorSource::UpdateComponentToTraceByName()
{
	if (ComponentToTraceName.GetStringLength() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComponentToTraceName too short!"));
		return false;
	}

	ComponentToTrace = UDataFactoryBPLibrary::FindFirstSceneComponentByName(this, ComponentToTraceName);


	UE_LOG(LogTemp, Warning, TEXT("Couldn't find a scene component with the ComponentToTraceName : %s!"), *(ComponentToTraceName.ToString()));
	return false;
}


// Called every frame
void ATraceActorSource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ComponentToTrace) return;

	FVector StartLocation = ComponentToTrace->GetComponentLocation();

	FVector EndLocation = GetTraceEndLocation();

	//UpdateTrace(LatestHits, StartLocation, EndLocation, FCollisionObjectQueryParams::AllObjects, FCollisionQueryParams());

	//static const FName LineTraceSingleName(TEXT("LineTraceSingle"));
	//FCollisionQueryParams Params = ConfigureCollisionParams(LineTraceSingleName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, this);
	//FCollisionObjectQueryParams ObjectParams = ConfigureCollisionObjectParams(ObjectTypes);

	if (GetWorld())
	{
		//GetWorld()->LineTraceMultiByObjectType(LatestHits, StartLocation, EndLocation, ObjectParams, Params);

		UKismetSystemLibrary::LineTraceMultiForObjects(
			this,
			StartLocation,
			EndLocation,
			ObjectTypes,
			bTraceComplex,
			ActorsToIgnore,
			DrawDebugTrace,
			LatestHits,
			bIgnoreSelf);
	}

	PostTickUpdates(DeltaTime);
	PostTickUpdatesBP(DeltaTime);
}

FVector ATraceActorSource::GetTraceEndLocation()
{
	if (!ComponentToTrace) return FVector(-1);
	FVector Direction;
	switch (TraceDirection)
	{
		case ValidTracerDirectionTypes::Forward:
			Direction = ComponentToTrace->GetForwardVector();
			break;

		case ValidTracerDirectionTypes::Backward:
			Direction = ComponentToTrace->GetForwardVector() * -1.0f;
			break;

		case ValidTracerDirectionTypes::Right:
			Direction = ComponentToTrace->GetRightVector();
			break;

		case ValidTracerDirectionTypes::Left:
			Direction = ComponentToTrace->GetRightVector() * -1.0f;
			break;

		case ValidTracerDirectionTypes::Up:
			Direction = ComponentToTrace->GetUpVector();
			break;

		case ValidTracerDirectionTypes::Down:
			Direction = ComponentToTrace->GetUpVector() * -1.0f;
			break;
	}

	return ComponentToTrace->GetComponentLocation() + Direction * LengthOfTheTrace;
}

