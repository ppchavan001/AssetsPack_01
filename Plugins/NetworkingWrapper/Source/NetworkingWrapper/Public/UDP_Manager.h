// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ObjectDeliverer/Public/ObjectDelivererManager.h>
#include <ObjectDeliverer/Public/DeliveryBox/ObjectDeliveryBoxUsingJson.h>
#include "UDP_Manager.generated.h"

UINTERFACE(MinimalAPI)
class UUDP_DataReceiverInterface : public UInterface {
	GENERATED_BODY()
};

class NETWORKINGWRAPPER_API IUDP_DataReceiverInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NETWORKING_WRAPPER | UDP")
		void OnUDP_StringDataReceived(const FString& Data);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NETWORKING_WRAPPER | UDP")
		void OnUDP_ObjectReceived(const UObject* const Data, const UClass* const DataClass);
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

	UFUNCTION(BlueprintCallable)
	void SendStringData(const FString& Data);
	
private:
	UObjectDelivererManager* DeliveryManager = NULL;
	UObjectDeliveryBoxUsingJson* DeliveryBox = NULL;

	UObjectDelivererProtocol* Protocol = NULL;
	UPacketRule* PacketRule = NULL;

	UPROPERTY(EditInstanceOnly)
	bool IsSender = true;

	UPROPERTY(EditInstanceOnly)
	UClass* JsonObjectClass = NULL;

	UPROPERTY(EditInstanceOnly)
	FString IP = "";

	UPROPERTY(EditInstanceOnly)
	int Port = 12429;


	UFUNCTION()
	void OnDataReceived(const UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer);
	
};
