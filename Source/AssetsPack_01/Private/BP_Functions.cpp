// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_Functions.h"

bool UBP_Functions::SegmentIntersection2DExposed(const FVector& SegmentStartA,
	const FVector& SegmentEndA,
	const FVector& SegmentStartB,
	const FVector& SegmentEndB,
	FVector& out_IntersectionPoint) 
{
	return FMath::SegmentIntersection2D(SegmentStartA, SegmentEndA, SegmentStartB, SegmentEndB, out_IntersectionPoint);
}