// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Runtime/LevelSequence/Public/LevelSequenceActor.h>
#include "SnapToSequencerCameraActor.generated.h"

UCLASS()
class ASnapToSequencerCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnapToSequencerCameraActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	ALevelSequenceActor* SequenceToFollow = NULL;

};
