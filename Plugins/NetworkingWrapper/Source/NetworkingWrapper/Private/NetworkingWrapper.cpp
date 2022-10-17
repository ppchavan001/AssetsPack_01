// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetworkingWrapper.h"
#include "Utils/JsonSerializer/ODJsonDeserializer.h"
#include "Utils/JsonSerializer/ODJsonSerializer.h"
#include "NetworkingInterface.h"

#define LOCTEXT_NAMESPACE "FNetworkingWrapperModule"

void FNetworkingWrapperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FNetworkingWrapperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

UObject* FNetworkingWrapperModule::String2UObject(const FString& string, UClass* classType)
{
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(string);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) return nullptr;

	UODJsonDeserializer* Deserializer = NewObject<UODJsonDeserializer>();
	UObject* createdObj = Deserializer->JsonObjectToUObject(JsonObject, classType);

	return createdObj;

}

FString FNetworkingWrapperModule::UObject2String(const UObject* packet)
{
	UODJsonSerializer* Serializer = NewObject<UODJsonSerializer>();
	auto jsonObject = Serializer->CreateJsonObject(packet);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), Writer);

	return OutputString;
}

void FNetworkingWrapperModule::UDP_SendString(const FString& Data)
{

}

void FNetworkingWrapperModule::UDP_SendObject(const UObject* Data)
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNetworkingWrapperModule, NetworkingWrapper)