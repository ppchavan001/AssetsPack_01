// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkingWrapperFunctionLibrary.h"

#include "Utils/JsonSerializer/ODJsonDeserializer.h"
#include "Utils/JsonSerializer/ODJsonSerializer.h"
#include "NetworkingInterface.h"
#include "Kismet/GameplayStatics.h"

UObject* UNetworkingWrapperFunctionLibrary::String2UObject(const FString& string, UClass* classType)
{
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(string);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) return nullptr;

	UODJsonDeserializer* Deserializer = NewObject<UODJsonDeserializer>();
	UObject* createdObj = Deserializer->JsonObjectToUObject(JsonObject, classType);

	return createdObj;

}

FString UNetworkingWrapperFunctionLibrary::UObject2String(const UObject* packet)
{
	UODJsonSerializer* Serializer = NewObject<UODJsonSerializer>();
	auto jsonObject = Serializer->CreateJsonObject(packet);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), Writer);

	return OutputString;
}

void UNetworkingWrapperFunctionLibrary::UDP_SendStr(const UObject* WorldContextObject, const FString& Data)
{
	TArray<AActor*> ActorsWithInterface;
	UGameplayStatics::GetAllActorsWithInterface(WorldContextObject, UUDP_DataSenderInterface::StaticClass(), ActorsWithInterface);

	for (AActor* Actor : ActorsWithInterface)
	{
		IUDP_DataSenderInterface::Execute_UDP_SendString(Actor, Data);
	}
}

void UNetworkingWrapperFunctionLibrary::UDP_SendObj(const UObject* WorldContextObject, const UObject* Data)
{
	TArray<AActor*> ActorsWithInterface;
	UGameplayStatics::GetAllActorsWithInterface(WorldContextObject, UUDP_DataSenderInterface::StaticClass(), ActorsWithInterface);

	for (AActor* Actor : ActorsWithInterface)
	{
		IUDP_DataSenderInterface::Execute_UDP_SendObject(Actor, Data);
	}
}
