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

	UFUNCTION(BlueprintPure, Category = "UnrealJSExtensions")
		UJavascriptIsolate* GetJSIsolate()
	{
		return  JavascriptIsolate;
	}


	/*
	*  if true will call activate on super which will try to execute the specified script
	* 
	* if false will not call super::Activate
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnrealJSExtensions")
	bool bExecuteSuperActivate = true;

	// Begin UActorComponent interface.
	virtual void Activate(bool bReset = false) override;
};
