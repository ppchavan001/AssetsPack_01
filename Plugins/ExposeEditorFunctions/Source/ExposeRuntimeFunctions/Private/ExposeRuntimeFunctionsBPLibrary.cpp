// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeRuntimeFunctionsBPLibrary.h"
#include "ExposeRuntimeFunctions.h"

UExposeRuntimeFunctionsBPLibrary::UExposeRuntimeFunctionsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FKey UExposeRuntimeFunctionsBPLibrary::ExposeRuntimeFunctionsSampleFunction(FName name)
{
	return FKey(name);
}
