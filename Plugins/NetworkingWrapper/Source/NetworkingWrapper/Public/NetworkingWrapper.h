// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FNetworkingWrapperModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	UFUNCTION(BlueprintCallable)
	static UObject* String2UObject(const FString& string, UClass* classType);

	UFUNCTION(BlueprintCallable)
	FString UObject2String(const UObject* packet);


};
