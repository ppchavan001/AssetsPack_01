// Fill out your copyright notice in the Description page of Project Settings.


#include "TracerActor.h"
#include "Engine/World.h"
#include "EngineUtils.h"

// Sets default values
ATracerActor::ATracerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetComponentToTraceByName();

}




bool ATracerActor::UpdateTrace(TArray<struct FHitResult>& OutHits, const FVector& Start, const FVector& End, const FCollisionObjectQueryParams& ObjectQueryParams, const FCollisionQueryParams& Params) const
{
	if (GetWorld())
		return GetWorld()->LineTraceMultiByObjectType(OutHits, Start, End, ObjectQueryParams, Params);

	return false;
}

bool ATracerActor::SetComponentToTraceByName()
{
	if (ComponentToTraceName.GetStringLength() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComponentToTraceName too short!"));
		return false;
	}
	
	if (UWorld* World = GetWorld())
	{
		for (TActorIterator<AActor> It(World, AActor::StaticClass()); It; ++It)
		{
			AActor* Actor = *It;
			
			if (Actor)
			{
				TSet<UActorComponent*> Components = Actor->GetComponents();

				for (UActorComponent* Component : Components)
				{
					if (Component->GetFName() == ComponentToTraceName)
					{
						if (USceneComponent* SceneComp = Cast<USceneComponent>(Component))
						{
							ComponentToTrace = SceneComp;
							return true;
						}

						UE_LOG(LogTemp, Warning, TEXT("ComponentToTraceName found but not a scene component!"));
					}
				}

			}

		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Couldn't find a scene component with the ComponentToTraceName!"));
	return false;
}

// Called when the game starts or when spawned
void ATracerActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATracerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ComponentToTrace) return;
	
	FVector StartLocation = ComponentToTrace->GetComponentLocation();

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

	FVector EndLocation = ComponentToTrace->GetComponentLocation() + Direction * LengthOfTheTrace;

	UpdateTrace(LatestHits, StartLocation, EndLocation);
}

