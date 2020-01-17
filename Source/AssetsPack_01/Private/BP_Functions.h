// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "BP_Functions.generated.h"

/**
 * 
 */
UCLASS()
class UBP_Functions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		UFUNCTION(BlueprintPure, BlueprintCallable, Category = "MathCustom")
		static bool SegmentIntersection2DExposed(const FVector& SegmentStartA,
			const FVector& SegmentEndA,
			const FVector& SegmentStartB,
			const FVector& SegmentEndB,
			FVector& out_IntersectionPoint);
};
