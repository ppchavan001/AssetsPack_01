// Fill out your copyright notice in the Description page of Project Settings.


#include "DataLoaderActorBackend.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Async/Async.h"
#include "Async/AsyncWork.h"

class DataLoaderAsyncTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<DataLoaderAsyncTask>;

	UObject* WorldContextObject;

	DataLoaderAsyncTask(UObject* Object)
		: WorldContextObject(Object)
	{}

	void DoWork()
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			auto lvls = World->GetLevels();
			for (ULevel* Level : lvls)
			{
				if (Level)
				{
					auto actrs = Level->Actors;
					for (auto Actor : actrs)
					{
						if (Actor && Actor->Implements<UDataLoaderInterface>())
						{
							IDataLoaderInterface::Execute_PostDataLoadCallback(Actor);
						}
					}
				}
			}


			return;
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

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(DataLoaderAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

// Sets default values
ADataLoaderActorBackend::ADataLoaderActorBackend()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ADataLoaderActorBackend::PostDataLoadingCallbackAsync()
{
	OnDataLoadingFinished.Broadcast();
	(new FAutoDeleteAsyncTask<DataLoaderAsyncTask>(this))->StartBackgroundTask();

}

void ADataLoaderActorBackend::GetAllObjectsWithTagCached(TArray<UObject*>& OutActors, const FName Tag, bool bForceRecache)
{
	if (bForceRecache || TagMapOfObjects.Num() == 0 || !TagMapOfObjects.Contains(Tag))
	{
		BuildTagMap();
	}

	if (TagMapOfObjects.Contains(Tag))
	{
		OutActors = TagMapOfObjects[Tag].Array();
	}
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

void ADataLoaderActorBackend::BuildTagMap()
{
	UWorld* World = GetWorld();

	if (World)
	{
		TagMapOfObjects.Reset();
	
		for (TActorIterator<AActor> It(World, AActor::StaticClass()); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor && !Actor->IsPendingKill())
			{
				for (auto Tag : Actor->Tags)
				{
					AddObjectToTagMap(Actor, Tag);
				}

				for (auto component : Actor->GetComponents())
				{
					for (auto Tag : component->ComponentTags)
					{
						AddObjectToTagMap(component, Tag);
					}
				}
			}
		}
	}
}

