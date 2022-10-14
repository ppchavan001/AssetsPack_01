// Fill out your copyright notice in the Description page of Project Settings.

#include "UDP_Manager.h"

#include "Protocol/ProtocolFactory.h"

#include "PacketRule/PacketRuleFactory.h"
#include "PacketRule/PacketRuleNodivision.h"

#include "Protocol/ProtocolUdpSocketSender.h"
#include "Protocol/ProtocolUdpSocketReceiver.h"

// Sets default values
AUDP_Manager::AUDP_Manager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to
	// improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DeliveryManager = UObjectDelivererManager::CreateObjectDelivererManager(true);
	DeliveryManager->ReceiveData.AddDynamic(this, &AUDP_Manager::OnDataReceived);


	// Protocol
	UObjectDelivererProtocol* Protocol = NULL;
	if (IsSender)
	{
		Protocol = UProtocolFactory::CreateProtocolUdpSocketSender(IP, Port);
	}
	else
	{
		Protocol = UProtocolFactory::CreateProtocolUdpSocketReceiver(Port);
	}

	DeliveryManager->Start(Protocol, UPacketRuleFactory::CreatePacketRuleNodivision());
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

void AUDP_Manager::OnDataReceived(const UObjectDelivererProtocol* ClientSocket,
	const TArray<uint8>& Buffer)
{

}
