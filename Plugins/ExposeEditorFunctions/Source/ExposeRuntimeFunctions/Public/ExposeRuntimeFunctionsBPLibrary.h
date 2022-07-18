// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputCore/Classes/InputCoreTypes.h"
#include "UObject/UnrealType.h"

#include "ExposeRuntimeFunctionsBPLibrary.generated.h"
/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/



UENUM(BlueprintType)
enum class Axis3D : uint8
{
	X UMETA(DisplayName = "X"),
	Y UMETA(DisplayName = "Y"),
	Z UMETA(DisplayName = "Z")
};

USTRUCT(BlueprintType)
struct FDeltaVertexRequiredInfo
{
	GENERATED_BODY()


public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		Axis3D Axis = Axis3D::Z;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxDeltaL = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxDeltaR = 1.0f;

};

UCLASS()
class UExposeRuntimeFunctionsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	
	UFUNCTION(BlueprintCallable, Category = "ExposeRuntimeFunctions | VP_Utils")
	static bool PlotVertices(const TArray<FVector>& VerticesToPlot);

	UFUNCTION(BlueprintCallable, Category = "ExposeRuntimeFunctions | VP_Utils")
	static float GetMaxZVal(const TArray<FVector>& Vertices);

	UFUNCTION(BlueprintCallable, Category = "ExposeRuntimeFunctions | VP_Utils")
	static void MassDebugDrawPoint(const TArray<FVector>& Vertices, const AActor* ParentActor, const float DrawPercentage = 25, const FVector DeltaLocation = FVector(0,0,0), float Size = 1.0f, FColor const& Color = FColor(255,255,0,255), bool bPersistentLines = false, float LifeTime = 2.0f, uint8 DepthPriority = 0);


	// TODO : Find a way to modify in place
	// Target channel 0 = X, 1 = Y, 2 = Z
	UFUNCTION(BlueprintCallable, Category = "ExposeRuntimeFunctions | VP_Utils")
	static TArray<FVector> AddDeltaToMatrixVertices(const TArray<FVector>& VerticesIn, const Axis3D TargetChannel = Axis3D::Z, const float MaxDeltaL = 1, const float MaxDeltaR = 1, const  int32 MatrixWidth = 0, bool InvertDelta = false);


	UFUNCTION(BlueprintCallable, Category = "ExposeRuntimeFunctions | VP_Utils")
		static TArray<FVector> MultiAddDeltaToMatrixVertices(const TArray<FVector>& VerticesIn, const TArray<FDeltaVertexRequiredInfo> DeltaVertexInfo, const  int32 MatrixWidth = 0, bool InvertDelta = false);


	UFUNCTION(BlueprintCallable, Category = "ExposeRuntimeFunctions | VP_Utils")
		static void GeneratePFMDataOnly(
			const FString& File,
			const FVector& StartLocation, const FRotator& StartRotation, const AActor* PFMOrigin,
			const int TilesHorizontal, const int TilesVertical, const float ColumnAngle,
			const float TileSizeHorizontal, const float TileSizeVertical, const int TilePixelsHorizontal,
			const int TilePixelsVertical, const bool AddMargin, const TArray<bool>& TilesValidityFlags, TArray<FVector>& PFMDataOut);


};
