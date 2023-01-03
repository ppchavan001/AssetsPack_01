// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataFactoryBPLibrary.h"
#include "DataLoaderActorBackend.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDataLoadingFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPreMasterDataLoadingDelegate);
DECLARE_DYNAMIC_DELEGATE(FAsyncPostLoadCallbackDelegate);

UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UDataLoaderInterface : public UInterface
{
	GENERATED_BODY()
};

class IDataLoaderInterface
{
	GENERATED_BODY()

public:
	/** Function called before any file processing has started.*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "DataFactory | Interface")
		void PreMasterDataLoadCallback();


	/** Function called after all data processing has finished and all target objects have received data from files.*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "DataFactory | Interface")
		void PostDataLoadCallback();
};

UCLASS(Abstract)
class DATAFACTORY_API ADataLoaderActorBackend : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADataLoaderActorBackend();

	// Not using async callback currently
	// Global callback : everyone who implements this function will be called
	// TODO : Targeted Object callback : call only modified objects.
	UFUNCTION(BlueprintCallable)
		void PostDataLoadingCallbackAsync();

	UFUNCTION(BlueprintCallable)
		void PreMasterDataLoadingCallback();

	UFUNCTION(BlueprintCallable)
		void GetAllObjectsWithTagCached(TArray<UObject*>& OutActors, const FName Tag, bool bForceRecache = false);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
		FOnDataLoadingFinished OnDataLoadingFinished;

	UPROPERTY(BlueprintAssignable)
		FPreMasterDataLoadingDelegate PreMasterDataLoadingDelegate;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters | Tags")
		FName ActionBindingTag = "SetActionBinding";


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters | Tags")
		FName AxisBindingTag = "SetAxisBinding";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters | Tags")
		FName KeyBindingTag = "SetKeyBinding";



	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<UObject*> TargetObjectsBackend;


	UFUNCTION(BlueprintCallable)
		void UpdatePropertyOnTargetObjects(const TArray<UObject*>& TargetObjects,
			const FName NameOfThePropertyToUpdate,
			const FString& DataToSet);

	void UpdateInputBinding(const FString& DataToSet, const TArray<UObject*>& TargetObjects, EInputBindingSupportedTypes InputBindingType);


	UFUNCTION(BlueprintCallable)
		void UpdateClassDefaults(const TSet<FName>& ClassNames,
			const FName NameOfThePropertyToUpdate,
			const FString& DataToSet);


	UFUNCTION(BlueprintPure)
		TArray<UObject*> GetTargetObjectsBackend(const TArray<UObject*> TargetObjectsOut);


private:
	// stores set of actors/components a the tag
	TMap<FName, TSet<UObject*>> TagMapOfObjects;
	void BuildTagMap();
	void AddObjectToTagMap(UObject* Object, FName Tag)
	{
		if (TagMapOfObjects.Contains(Tag))
		{
			TagMapOfObjects[Tag].Add(Object);
		}
		else
		{
			TagMapOfObjects.Add(Tag);
			TagMapOfObjects[Tag].Add(Object);
		}
	}

	EInputBindingSupportedTypes GetInputBindingType(const FName NameOfThePropertyToUpdate);
};
