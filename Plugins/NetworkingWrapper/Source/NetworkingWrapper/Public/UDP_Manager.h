// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDP_Manager.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UUDP_DataReceiverInterface : public UInterface {
	GENERATED_BODY()
};

class NETWORKINGWRAPPER_API IUDP_DataReceiverInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnUDP_StringDataReceived(FString& DataOut);


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	UObject* OnUDP_ObjectReceived();
};

UCLASS()
class NETWORKINGWRAPPER_API AUDP_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUDP_Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
