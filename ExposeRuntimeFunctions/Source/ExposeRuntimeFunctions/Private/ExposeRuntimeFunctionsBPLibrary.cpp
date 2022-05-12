// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeRuntimeFunctionsBPLibrary.h"
#include "ExposeRuntimeFunctions.h"

UExposeRuntimeFunctionsBPLibrary::UExposeRuntimeFunctionsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UExposeRuntimeFunctionsBPLibrary::ExposeRuntimeFunctionsSampleFunction(float Param)
{
	return -1;
}

