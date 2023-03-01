// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TraceActorSource.generated.h"


UENUM(BlueprintType)
enum class ValidTracerDirectionTypes : uint8
{
	Forward = 0,
	Backward = 1,

	Right = 2,
	Left = 3,

	Up = 4,
	Down = 5
};


UCLASS()
class ATraceActorSource : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATraceActorSource();



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tracer)
		TArray<struct FHitResult> LatestHits;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracer)
		ValidTracerDirectionTypes TraceDirection = ValidTracerDirectionTypes::Forward;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracer)
		float LengthOfTheTrace = 500;

	/*bool UpdateTrace(TArray<struct FHitResult>& OutHits,
					 const FVector& Start,
					 const FVector& End,
					 const FCollisionObjectQueryParams& ObjectQueryParams,
					 const FCollisionQueryParams& Params) const;*/

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Tracer)
		void SetComponentToTrace(USceneComponent* Component)
	{
		ComponentToTrace = Component;
	}

	// might not the name of the component
	// This name is used to search the world for a component with this name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracer)
		FName ComponentToTraceName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracer)
		bool bTraceComplex = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracer)
		bool bIgnoreSelf = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracer)
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracer)
		TArray<AActor*> ActorsToIgnore;

	UFUNCTION(BlueprintCallable, Category = Tracer)
		bool UpdateComponentToTraceByName();

	UFUNCTION(BlueprintCallable, Category = Tracer)
		void SetComponentToTraceName(FName ComponentName)
	{
		ComponentToTraceName = ComponentName;
		UpdateComponentToTraceByName();
	}

	UFUNCTION(BlueprintPure, Category = Tracer)
		USceneComponent* GetComponentToTrace()
	{
		return ComponentToTrace;
	}

	UFUNCTION(BlueprintPure, Category = Tracer)
		FVector GetTraceEndLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracer, AdvancedDisplay)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTrace = EDrawDebugTrace::None;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	USceneComponent* ComponentToTrace;

};
