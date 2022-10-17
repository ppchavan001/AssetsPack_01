// Fill out your copyright notice in the Description page of Project Settings.

#include "UDP_Manager.h"

#include "Protocol/ProtocolFactory.h"

#include "PacketRule/PacketRuleFactory.h"
#include "PacketRule/PacketRuleNodivision.h"

#include "Protocol/ProtocolUdpSocketSender.h"
#include "Protocol/ProtocolUdpSocketReceiver.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/ODStringUtil.h"
#include <Runtime/Engine/Public/EngineUtils.h>
#include "NetworkingWrapper.h"
#include "Logging/LogMacros.h"
#include "NetworkingWrapperFunctionLibrary.h"

// Sets default values
AUDP_Manager::AUDP_Manager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to
	// improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AUDP_Manager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AUDP_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUDP_Manager::SetUpUDPManager(TArray<UClass*> _TargetObjectClass, bool _IsSender /*= true*/, FString _IP /*= "localhost"*/, int _Port /*= 12429*/)
{
	IsSender = _IsSender;
	IP = _IP;
	Port = _Port;
	TargetObjectClass = _TargetObjectClass;


	DeliveryManager = UObjectDelivererManager::CreateObjectDelivererManager(true);
	DeliveryManager->ReceiveData.AddDynamic(this, &AUDP_Manager::OnDataReceived);


	// Protocol
	if (IsSender)
	{
		Protocol = UProtocolFactory::CreateProtocolUdpSocketSender(IP, Port);
	}
	else
	{
		Protocol = UProtocolFactory::CreateProtocolUdpSocketReceiver(Port);
	}

	PacketRule = UPacketRuleFactory::CreatePacketRuleNodivision();
	DeliveryBox = UDeliveryBoxFactory::CreateUtf8StringDeliveryBox(); //UDeliveryBoxFactory::CreateObjectDeliveryBoxUsingJson(JsonObjectClass);

	DeliveryManager->Start(Protocol, PacketRule, DeliveryBox);



}

void AUDP_Manager::UDP_SendString_Implementation(const FString& Data)
{

	// if this object is not a sender,
	// return
	if (!IsSender) return;

	// if this object setup is not complete,
	// return
	if (!DeliveryManager)
	{
		UE_LOG(LogTemp, Error, TEXT("DeliveryManager is not setup! Please call SetUpUDPManager before sending/ receiving any data."));
		return;
	}

	TArray<uint8> buffer;
	UODStringUtil::StringToBuffer(Data, buffer);
	DeliveryManager->Send(buffer);
}

void AUDP_Manager::UDP_SendObject_Implementation(const UObject* const Data)
{
	if (!IsSender) return;

	this->UDP_SendString_Implementation(UNetworkingWrapperFunctionLibrary::UObject2String(Data));
}

void AUDP_Manager::OnDataReceived(const UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
{
	TArray<AActor*> ActorsWithInterface;
	UGameplayStatics::GetAllActorsWithInterface(this, UUDP_DataReceiverInterface::StaticClass(), ActorsWithInterface);

	FString ReceivedString = UODStringUtil::BufferToString(Buffer);


	for (auto objClass : TargetObjectClass)
	{
		UObject* JsonObj = UNetworkingWrapperFunctionLibrary::String2UObject(ReceivedString, objClass);

		if (JsonObj)
		{
			/*
			* received json object
			* call callback and return
			*/

			for (AActor* Actor : ActorsWithInterface)
			{
				IUDP_DataReceiverInterface::Execute_OnUDP_ObjectReceived(Actor, JsonObj, objClass);
			}

			return;
		}
	}

	/*
	* if couldn't cast to json obj,
	* call string callback
	*/
	for (AActor* Actor : ActorsWithInterface)
	{
		IUDP_DataReceiverInterface::Execute_OnUDP_StringDataReceived(Actor, ReceivedString);
	}

}
