// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NetworkingWrapperFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class NETWORKINGWRAPPER_API UNetworkingWrapperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	* tries to convert given string to a object of given class
	* the string must be json representation of the object
	*/
	UFUNCTION(BlueprintCallable)
		static UObject* String2UObject(const FString& string, UClass* classType);

	/*
	* Converts the given object to a json string
	*/
	UFUNCTION(BlueprintCallable)
		static FString UObject2String(const UObject* packet);

	/*
	* calls UDP_Send on all objects with UDP Send interface
	* with given parameters
	*/
	UFUNCTION(BlueprintCallable, Category = "Networking Wrapper | UDP")
		static void UDP_SendStr(const UObject* WorldContextObject, const FString& Data);

	/*
	* calls UDP_Send on all objects with UDP Send interface 
	* with given parameters
	*/
	UFUNCTION(BlueprintCallable, Category = "Networking Wrapper | UDP")
		static void UDP_SendObj(const UObject* WorldContextObject, const UObject* Data);
};
