// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "PFMUtilsBPLibrary.generated.h"

UENUM(BlueprintType)
enum class EAxis3D : uint8
{
	X = 0,
	Y = 1,
	Z = 2
};


// Valid range is all columns/ rows greater (>) than start and less (<) than end
USTRUCT(BlueprintType)
struct FDeltaVerticesRequiredData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDelta = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAxis3D TargetChannel = EAxis3D::Z;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MatrixWidth = 1280;
	

	// Inclusive
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StartColumn = 0;

	// Inclusive
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StartRow = 0;

	// Non inclusive
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EndColumn = 1280;

	// Non inclusive
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EndRow = 720;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeFalloffRow = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeFalloffColumn = 1.0f;


};


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
UCLASS()
class UPFMUtilsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()



	/*
	* String Input:
		X=-0.000 Y=21.332 Z=-0.476
		X=-0.000 Y=22.324 Z=-0.476
		X=-0.000 Y=23.316 Z=-0.476


		Output
		Array of vectors with 3 elements

		Note: Will clear VerticesOut before filling it with vectors
	*/
	UFUNCTION(BlueprintCallable, Category = "PFM Utils BPLibrary | Import/ Export")
	static void ConvertStringToVector(TArray<FString> Lines, TArray<FVector>& VerticesOut, const bool IsMatrix = true);


	UFUNCTION(BlueprintCallable, Category = "PFM Utils BPLibrary | Import/ Export")
	static FString ConvertVectorArrayToString(TArray<FString>& LinesOut, TArray<FVector> Vertices);


	// Will only export 
	// x/y/z of extreme vertices
	UFUNCTION(BlueprintCallable, Category = "PFM Utils BPLibrary | Import/ Export")
	static FString ConvertMatrixArrayToString(const TArray<FVector> Vertices);

	UFUNCTION(BlueprintCallable, Category = "PFM Utils BPLibrary | Visualization")
	static void MassDebugDrawPoint(const TArray<FVector>& Vertices, const AActor* ParentActor, const float DrawPercentage = 25, const FVector DeltaLocation = FVector(0, 0, 0), float Size = 1.0f, FColor const& Color = FColor(255, 255, 0, 255), bool bPersistentLines = false, float LifeTime = 2.0f, uint8 DepthPriority = 0);

	// TODO : Find a way to modify in place
	// Target channel 0 = X, 1 = Y, 2 = Z
	UFUNCTION(BlueprintCallable, Category = "PFM Utils BPLibrary | Visualization")
	static TArray<FVector> AddDeltaToMatrixVertices(
		const TArray<FVector>& VerticesIn, 
		const FDeltaVerticesRequiredData DeltaVerticesData,
		bool InvertDelta = false);


	UFUNCTION(BlueprintCallable, Category = "PFM Utils BPLibrary | Import/ Export")
	static void GeneratePFMDataOnly(
			const FString& File,
			const FVector& StartLocation, const FRotator& StartRotation, const AActor* PFMOrigin,
			const int TilesHorizontal, const int TilesVertical, const float ColumnAngle,
			const float TileSizeHorizontal, const float TileSizeVertical, const int TilePixelsHorizontal,
			const int TilePixelsVertical, const bool AddMargin, const TArray<bool>& TilesValidityFlags, TArray<FVector>& PFMDataOut);



	UFUNCTION(BlueprintCallable, Category = "PFM Utils BPLibrary | Import/ Export")
	static bool WriteStringToFile(const FString FileName, const FString DataToWrite);


	UFUNCTION(BlueprintGetter, Category = "PFM Utils BPLibrary | Import/ Export")
	static TArray<FString> ReadLinesFromFile(const FString FileName);

	UFUNCTION(BlueprintCallable, Category = "PFM Utils BPLibrary | Import/ Export")
	static bool WriteVerticesToFile(const FString FileName, const TArray<FVector> Vertices, const bool MatrixOptimization = true);

};
