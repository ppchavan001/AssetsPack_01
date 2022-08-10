// Fill out your copyright notice in the Description page of Project Settings.


#include "DataLoaderActorBackend.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
//#include "Async/Async.h"

// Sets default values
ADataLoaderActorBackend::ADataLoaderActorBackend()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ADataLoaderActorBackend::PostDataLoadingCallback()
{
	PostDataLoadingCallback_Internal();

}

// Called when the game starts or when spawned
void ADataLoaderActorBackend::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADataLoaderActorBackend::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADataLoaderActorBackend::PostDataLoadingCallback_Internal()
{
	OnDataLoadingFinished.Broadcast();

	if (UWorld* World = GetWorld())
	{
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->Implements<UDataLoaderInterface>())
			{
				IDataLoaderInterface::Execute_PostDataLoadCallback(Actor);
			}
		}
	}
}

