// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JavascriptComponent.h"
#include "JSX_Javascript.generated.h"

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALJSEXTENSIONS_API UJSX_Javascript : public UJavascriptComponent
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintPure, Category = "UnrealJSExtensions")
		UJavascriptContext* GetJSContext()
	{
		return JavascriptContext;
	}
};
