// Fill out your copyright notice in the Description page of Project Settings.

#include "UDP_Manager.h"

#include "Protocol/ProtocolFactory.h"

#include "PacketRule/PacketRuleFactory.h"
#include "PacketRule/PacketRuleNodivision.h"

#include "Protocol/ProtocolUdpSocketSender.h"
#include "Protocol/ProtocolUdpSocketReceiver.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/ODStringUtil.h"

// Sets default values
AUDP_Manager::AUDP_Manager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to
	// improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	DeliveryBox = UDeliveryBoxFactory::CreateObjectDeliveryBoxUsingJson(JsonObjectClass);

	DeliveryManager->Start(Protocol, PacketRule, DeliveryBox);
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

void AUDP_Manager::SendStringData(const FString& Data)
{
	TArray<uint8> buffer;
	UODStringUtil::StringToBuffer(Data, buffer);
	DeliveryManager->Send(buffer);
}

void AUDP_Manager::OnDataReceived(const UObjectDelivererProtocol* ClientSocket,
	const TArray<uint8>& Buffer)
{
	TArray<AActor*> ActorsWithInterface;
	UGameplayStatics::GetAllActorsWithInterface(this, UUDP_DataReceiverInterface::StaticClass(), ActorsWithInterface);

	FString ReceivedString = BytesToString(&Buffer[0], Buffer.Num());

	for (AActor* Actor : ActorsWithInterface)
	{
		IUDP_DataReceiverInterface* UDPInterface = Cast<IUDP_DataReceiverInterface>(Actor);
		if (UDPInterface)
		{
			UDPInterface->OnUDP_StringDataReceived(ReceivedString);
		}
	}
}
