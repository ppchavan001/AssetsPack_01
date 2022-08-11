// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataLoaderActorBackend.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDataLoadingFinished);
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
	/** Add interface function declarations here */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="DataFactory | Interface")
	void PostDataLoadCallback();
};

UCLASS()
class DATAFACTORY_API ADataLoaderActorBackend : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADataLoaderActorBackend();

	UFUNCTION(BlueprintCallable)
	void PostDataLoadingCallbackAsync(bool bUseAsync = false);


	UFUNCTION(BlueprintCallable)
	void GetAllObjectsWithTagCached(TArray<UObject*>& OutActors, const FName Tag, bool bForceRecache = false);

	//UFUNCTION(BlueprintCallable)
	//void GetActorsWithTag(Fs);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnDataLoadingFinished OnDataLoadingFinished;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters | Tags")
	FName ActionBindingTag = "SetActionBinding";


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters | Tags")
		FName AxisBindingTag = "SetAxisBinding";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters | Tags")
		FName KeyBindingTag = "SetKeyBinding";

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

	UFUNCTION(BlueprintCallable)
		void UpdatePropertyOnTargetObjects(const TArray<UObject*>& TargetObjects, 
										   const FName NameOfThePropertyToUpdate, 
										   const FString& DataToSet);


	UFUNCTION(BlueprintPure)
		TArray<UObject*> GetTargetObjectsBackend(const TArray<UObject*> TargetObjectsOut);



};
