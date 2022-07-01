// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TracerActor.generated.h"

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
class EXPOSERUNTIMEFUNCTIONS_API ATracerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATracerActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<struct FHitResult> LatestHits;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ValidTracerDirectionTypes TraceDirection = ValidTracerDirectionTypes::Forward;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LengthOfTheTrace = 500;

	bool UpdateTrace(TArray<struct FHitResult>& OutHits, const FVector& Start, const FVector& End, const FCollisionObjectQueryParams& ObjectQueryParams = FCollisionObjectQueryParams::AllObjects, const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam) const;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetComponentToTrace(USceneComponent* Component)
	{
		ComponentToTrace = Component;
	}

	// might not the name of the component
	// This name is used to search the world for a component with this name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ComponentToTraceName = "";

	UFUNCTION(BlueprintCallable)
	bool SetComponentToTraceByName();

	UFUNCTION(BlueprintGetter)
	USceneComponent* GetComponentToTrace()
	{
		return ComponentToTrace;
	}



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	USceneComponent* ComponentToTrace;

};
