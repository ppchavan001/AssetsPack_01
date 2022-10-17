// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ObjectDeliverer/Public/ObjectDelivererManager.h>
#include <ObjectDeliverer/Public/DeliveryBox/ObjectDeliveryBoxUsingJson.h>
#include "DeliveryBox/Utf8StringDeliveryBox.h"
#include "NetworkingInterface.h"
#include "UDP_Manager.generated.h"

UCLASS()
class NETWORKINGWRAPPER_API AUDP_Manager : public AActor, public IUDP_DataSenderInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUDP_Manager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable)
	void UDP_SendString_Implementation(const FString& Data);


	//UFUNCTION(BlueprintCallable)
	void UDP_SendObject_Implementation(const UObject* const Data);

	/*
	* On object received will only be triggered if this ObjectClass matches received string object class
	*/
	UFUNCTION(BlueprintCallable)
		void SetUpUDPManager(TArray<UClass*> _TargetObjectClass, bool _IsSender = true, FString _IP = "localhost", int _Port = 12429);

private:
	UObjectDelivererManager* DeliveryManager = NULL;
	UUtf8StringDeliveryBox* DeliveryBox = NULL;

	UObjectDelivererProtocol* Protocol = NULL;
	UPacketRule* PacketRule = NULL;

	bool IsSender = true;

	TArray<UClass*> TargetObjectClass;

	FString IP = "";

	int Port = 12429;


	UFUNCTION()
		void OnDataReceived(const UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer);

};
