// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkingWrapperFunctionLibrary.h"

#include "Utils/JsonSerializer/ODJsonDeserializer.h"
#include "Utils/JsonSerializer/ODJsonSerializer.h"
#include "NetworkingInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"

#define  CurrentFileName FString("NetworkingWrapperFunctionLibrary.cpp")
DEFINE_LOG_CATEGORY_STATIC(NETWRAPLOG, Log, All);


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


void UNetworkingWrapperFunctionLibrary::NW_PrintString(
	const UObject* WorldContextObject,
	const FString InString /*= ""*/,
	ENetWrapLogVerbosity LogVerbosity /*= EDataFactoryLogVerbosity::Log*/,
	bool bPrintToScreen /*= true*/,
	bool bPrintToLog /*= true*/,
	float Duration /*= 2.f*/,
	int MaxStackDataDepth  /* = 1*/)
{
	if (LogVerbosity == ENetWrapLogVerbosity::NoLogging)
	{
		return;
	}

#pragma region Prefix
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	FString Prefix;
	if (World)
	{
		if (World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				// GPlayInEditorID 0 is always the server, so 1 will be first client.
				// You want to keep this logic in sync with GeneratePIEViewportWindowTitle and UpdatePlayInEditorWorldDebugString
				Prefix = FString::Printf(TEXT("Client %d"), GPlayInEditorID);
				break;
			case NM_DedicatedServer:
			case NM_ListenServer:
				Prefix = FString::Printf(TEXT("Server"));
				break;
			case NM_Standalone:
				break;
			}
		}
	}


#pragma  endregion

	FString FinalLogString = Prefix + InString;

#pragma region AddCallerInfo

	static const FBoolConfigValueHelper DisplayPrintStringSource(TEXT("Kismet"), TEXT("bLogPrintStringSource"), GEngineIni);
	if (DisplayPrintStringSource)
	{

#if DO_BLUEPRINT_GUARD
		FString StackData;
		FBlueprintContextTracker& BlueprintExceptionTracker = FBlueprintContextTracker::Get();
		if (BlueprintExceptionTracker.GetScriptStack().Num() > 0)
		{
			int stackDepth = 0;

			for (int i = BlueprintExceptionTracker.GetScriptStack().Num() - 1; i > -1 && stackDepth < MaxStackDataDepth; --i)
			{
				StackData += BlueprintExceptionTracker.GetScriptStack()[i]->GetStackDescription();
				stackDepth += 1;

				if (stackDepth != MaxStackDataDepth)
				{
					StackData += " <- ";
				}

			}

			FinalLogString = "[" + StackData + "] " + FinalLogString;
		}
		else
		{
			//ScriptStack += TEXT("\t[Empty] (FFrame::GetScriptCallstack() called from native code)");
			//const FString SourceObjectPrefix = FString::Printf(TEXT("[%s]"), *GetNameSafe(WorldContextObject));
			FinalLogString = WorldContextObject->GetName() + FinalLogString;

		}

		// add log verbosity to final log? is added by ue on some verbosities
		//FinalLogString = "[" + FString(ToString((ELogVerbosity::Type)(LogVerbosity))) + "] " + FinalLogString;

#else
		//FString TopOfStack = TEXT("Unable to display Script Callstack. Compile with DO_BLUEPRINT_GUARD=1");
		FinalLogString = WorldContextObject->GetName() + FinalLogString;

#endif

	}

#pragma endregion

	// Log
#if !NO_LOGGING

	if (bPrintToLog)
	{
		GLog->Log(NETWRAPLOG.GetCategoryName(), (ELogVerbosity::Type)(LogVerbosity), &FinalLogString[0]);
	}
#endif
	// Also output to the screen, if possible
	if (bPrintToScreen)
	{
		if (GAreScreenMessagesEnabled)
		{

			FColor OnScreenTextColor = FColor::White;

			switch (LogVerbosity)
			{
			case ENetWrapLogVerbosity::NoLogging:
				OnScreenTextColor = FColor(0, 0, 0, 0);
				break;
			case ENetWrapLogVerbosity::Fatal:
				OnScreenTextColor = FColor(255, 0, 0, 255);
				break;
			case ENetWrapLogVerbosity::Error:
				OnScreenTextColor = FColor(255, 25, 25, 255);
				break;
			case ENetWrapLogVerbosity::Warning:
				OnScreenTextColor = FColor(255, 255, 0, 255);
				break;
			case ENetWrapLogVerbosity::Display:
				OnScreenTextColor = FColor(255, 255, 255, 255);
				break;
			case ENetWrapLogVerbosity::Log:
				OnScreenTextColor = FColor(230, 230, 230, 255);
				break;
			case ENetWrapLogVerbosity::Verbose:
				OnScreenTextColor = FColor(200, 200, 200, 255);
				break;
			case ENetWrapLogVerbosity::VeryVerbose:
				OnScreenTextColor = FColor(150, 150, 150, 255);
				break;
			}


			GEngine->AddOnScreenDebugMessage((uint64)-1, Duration, OnScreenTextColor, FinalLogString);
		}
		else
		{
			UE_LOG(NETWRAPLOG, VeryVerbose, TEXT("%s : %s : %s : Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."),
				*CurrentFileName, *FString(__func__), __LINE__);
		}
	}
}
