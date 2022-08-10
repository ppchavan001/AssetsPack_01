// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataLoaderActorBackend.generated.h"


UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UDataLoaderInterface : public UInterface
{
	GENERATED_BODY()
};

class IDataLoaderInterface
{
	GENERATED_BODY()

public:
	/** Add interface function declarations here */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="DataFactory : Interface")
	void PostDataLoadCallback();
};

UCLASS()
class DATAFACTORY_API ADataLoaderActorBackend : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADataLoaderActorBackend();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
