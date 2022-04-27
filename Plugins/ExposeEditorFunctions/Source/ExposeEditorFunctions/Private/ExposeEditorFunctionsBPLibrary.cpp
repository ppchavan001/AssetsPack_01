// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeEditorFunctionsBPLibrary.h"
#include "ExposeEditorFunctions.h"

UExposeEditorFunctionsBPLibrary::UExposeEditorFunctionsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


#if WITH_EDITOR

void UExposeEditorFunctionsBPLibrary::SetStaticMeshLightMapResolution(UStaticMeshComponent* TargetMesh, int NewRes)
{
	TargetMesh->bOverrideLightMapRes = 1;
	TargetMesh->OverriddenLightMapRes = NewRes;
	TargetMesh->InvalidateLightingCacheDetailed(true, false);
}

#endif

