// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NetworkingInterface.generated.h"

/*********************************************
* UDP Receiver Interface
**********************************************/
UINTERFACE(MinimalAPI)
class UUDP_DataReceiverInterface : public UInterface
{
	GENERATED_BODY()
};

class NETWORKINGWRAPPER_API IUDP_DataReceiverInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Networking Wrapper | UDP")
		void OnUDP_StringDataReceived(const FString& Data);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Networking Wrapper | UDP")
		void OnUDP_ObjectReceived(const UObject* const Data, const UClass* const DataClass);
};




/*********************************************
* UDP Sender Interface
**********************************************/

UINTERFACE(MinimalAPI)
class UUDP_DataSenderInterface : public UInterface
{
	GENERATED_BODY()
};

class NETWORKINGWRAPPER_API IUDP_DataSenderInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Networking Wrapper | UDP")
		void UDP_SendString(const FString& Data);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Networking Wrapper | UDP")
		void UDP_SendObject(const UObject* const Data);
};

