// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VertexManager.generated.h"

UCLASS()
class PFMUTILS_API AVertexManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVertexManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool LoadVertices(FString FileName);


	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> Vertices;

};
