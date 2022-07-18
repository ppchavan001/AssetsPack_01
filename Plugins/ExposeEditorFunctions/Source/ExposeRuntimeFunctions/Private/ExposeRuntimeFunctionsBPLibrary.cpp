// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExposeRuntimeFunctionsBPLibrary.h"
#include "ExposeRuntimeFunctions.h"
#include "UObject/PropertyAccessUtil.h"
#include "UObject/UnrealType.h"

#include <string>
#include "Logging/LogMacros.h"
#include "DrawDebugHelpers.h"

UExposeRuntimeFunctionsBPLibrary::UExposeRuntimeFunctionsBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}






bool UExposeRuntimeFunctionsBPLibrary::PlotVertices(const TArray<FVector>& VerticesToPlot)
{
	return false;
}

float UExposeRuntimeFunctionsBPLibrary::GetMaxZVal(const TArray<FVector>& Vertices)
{
	float maxZ = -99999999999;
	bool ResultIsValid = false;

	for (auto Vertex : Vertices)
	{
		if (Vertex.Z > maxZ)
		{
			maxZ = Vertex.Z;
			ResultIsValid = true;
		}
	}

	return maxZ;
}



void UExposeRuntimeFunctionsBPLibrary::MassDebugDrawPoint(const TArray<FVector>& Vertices, const AActor* ParentActor, const float DrawPercentage /*= 25*/, const FVector DeltaLocation /*= FVector(0,0,0)*/, float Size /*= 1.0f*/, FColor const& Color /*= FColor(255,255,0,255)*/, bool bPersistentLines /*= false*/, float LifeTime /*= 2.0f*/, uint8 DepthPriority /*= 0*/)
{
	if (!ParentActor || !ParentActor->GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("ExposedEditorPlugin [MassDebugDrawPoint]: Invalid parent actor or world!"));
		return;
	}

	float DrawRatio = DrawPercentage / 100.0f;

	int32 DrawCount = static_cast<int32>(FMath::Floor(Vertices.Num() * DrawRatio));
	int32 SkipCount = Vertices.Num() - DrawCount;
	int32 gcd = 0;
	
	do
	{
		gcd = FMath::GreatestCommonDivisor(DrawCount, SkipCount);

		if (gcd < 1) break;

		DrawCount /= gcd;
		SkipCount /= gcd;
						 
	} while (gcd > 1 && DrawCount > 1 && SkipCount > 1);

	int i = 0;
	int32 SlotLength = DrawCount + SkipCount;
	while (i < Vertices.Num())
	{	
		for (int k = 0; k < SlotLength; ++k)
		{
			if (k < DrawCount)
			{
				auto Position = Vertices[i];
				DrawDebugPoint(ParentActor->GetWorld(), Position + DeltaLocation, Size, Color, bPersistentLines, LifeTime, DepthPriority);
			}
			
			
			++i;
		}
	}
}


#pragma optimize( "", on )
TArray<FVector> UExposeRuntimeFunctionsBPLibrary::AddDeltaToMatrixVertices(const TArray<FVector>& VerticesIn, const Axis3D TargetChannel, const float MaxDeltaL, const float MaxDeltaR, const int32 MatrixWidth, bool InvertDelta)
{
	TArray<FVector> VecArr;

	for (int i = 0; i < VerticesIn.Num(); ++i)
	{
		// get column number
		auto ColumnNumber = i % MatrixWidth; 

		// Converts the column number to 0 - 1 range
		auto Alpha = ColumnNumber / (float)MatrixWidth;

		// range 0 - 180
		auto AlphaAngle = Alpha * 180;

		// converting range to smooth curve
		auto FinalAlphaAngle = FMath::Sin(PI / (180.f) * AlphaAngle);

		auto MaxDelta = Alpha > 0.5 ? MaxDeltaR : MaxDeltaL;

		auto Delta = FMath::Lerp(MaxDelta, (float)0, FinalAlphaAngle);

		if (InvertDelta) Delta = 1 - Delta;

		auto vec = VerticesIn[i];

		if (i < MatrixWidth) UE_LOG(LogTemp, Warning, TEXT("Delta : %f"), Delta);
		
		switch (TargetChannel)
		{
			case Axis3D::X:
				vec.X += Delta;
				break;

			case Axis3D::Y:
				vec.Y += Delta;
				break;

			case Axis3D::Z:
				vec.Z += Delta;
				break;
		}

		VecArr.Add(vec);


	}





	return VecArr;
}

TArray<FVector> UExposeRuntimeFunctionsBPLibrary::MultiAddDeltaToMatrixVertices(const TArray<FVector>& VerticesIn, const TArray<FDeltaVertexRequiredInfo> DeltaVertexInfo, const int32 MatrixWidth /*= 0*/, bool InvertDelta /*= false*/)
{
	TArray<FVector> VecArr = VerticesIn;


	for (FDeltaVertexRequiredInfo Info : DeltaVertexInfo);
	VecArr = AddDeltaToMatrixVertices(VecArr, Info.) 


	return VecArr;

}

#pragma optimize( "", on )

void UExposeRuntimeFunctionsBPLibrary::GeneratePFMDataOnly(const FString& File, const FVector& StartLocation, const FRotator& StartRotation, const AActor* PFMOrigin, const int TilesHorizontal, const int TilesVertical, const float ColumnAngle, const float TileSizeHorizontal, const float TileSizeVertical, const int TilePixelsHorizontal, const int TilePixelsVertical, const bool AddMargin, const TArray<bool>& TilesValidityFlags, TArray<FVector>& PFMDataOut)
{
	// Check all input data
	if (File.IsEmpty() || !PFMOrigin || TilesHorizontal < 1 || TilesVertical < 1 || TileSizeHorizontal < 1.f || TileSizeVertical < 1.f || TilePixelsHorizontal < 2 || TilePixelsVertical < 2 || TilesValidityFlags.Num() != TilesHorizontal * TilesVertical)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong input data"));
	}

	// Explicit XYZ set to NAN instead of FVector(NAN, NAN, NAN) allows to avoid any FVector internal NAN checks
	FVector NanVector;
	NanVector.X = NanVector.Y = NanVector.Z = NAN;

	// Texture XY size
	const int TexSizeX = TilesHorizontal * TilePixelsHorizontal;
	const int TexSizeY = TilesVertical * TilePixelsVertical;

	// Prepare array for output data
	PFMDataOut.Reserve(TexSizeX * TexSizeY);

	// Some constants for navigation math
	const FTransform StartFrom = FTransform(StartRotation, StartLocation);
	const FVector RowTranslate(0.f, 0.f, -TileSizeVertical);
	const FVector ColTranslate(0.f, TileSizeHorizontal, 0.f);

	// Compute horizontal margins and pixel offsets
	const float PixelOffsetX = (AddMargin ? TileSizeHorizontal / TilePixelsHorizontal : TileSizeHorizontal / (TilePixelsHorizontal - 1));
	const float MarginX = (AddMargin ? PixelOffsetX / 2.f : 0.f);
	// Compute vertical margins and pixel offsets
	const float PixelOffsetY = (AddMargin ? TileSizeVertical / TilePixelsVertical : TileSizeVertical / (TilePixelsVertical - 1));
	const float MarginY = (AddMargin ? PixelOffsetY / 2.f : 0.f);

	// Cache tile transforms so we won't have to compute it for every pixel
	TMap<int, TMap<int, FTransform>> TransformsCache;

	// The order is from left to right, from top to bottom
	for (int Y = 0; Y < TexSizeY; ++Y)
	{
		// Current row index
		const int CurRow = Y / TilePixelsVertical;
		// Transform for current row
		const FTransform CurRowTransform = (CurRow == 0 ? StartFrom : FTransform(TransformsCache[CurRow - 1][0].Rotator(), TransformsCache[CurRow - 1][0].TransformPosition(RowTranslate)));

		// Cache current row transform
		TransformsCache.Emplace(CurRow);
		TransformsCache[CurRow].Emplace(0, CurRowTransform);

		for (int X = 0; X < TexSizeX; ++X)
		{
			// Current column index
			const int CurCol = X / TilePixelsHorizontal;
			// Check if the column transform has been cached previously
			const bool bCached = TransformsCache[CurRow].Contains(CurCol);
			// Transform of the current tile (top left corner)
			const FTransform CurTileTransform = (bCached ?
												 TransformsCache[CurRow][CurCol] :
												 (CurCol == 0 ? CurRowTransform : FTransform(TransformsCache[CurRow][CurCol - 1].Rotator().Add(0.f, ColumnAngle, 0.f), TransformsCache[CurRow][CurCol - 1].TransformPosition(ColTranslate))));

											 // Cache new transform
			if (!bCached)
			{
				TransformsCache[CurRow].Emplace(CurCol, CurTileTransform);
			}

			// XY within a tile
			const int TilePixelY = Y % TilePixelsVertical;
			const int TilePixelX = X % TilePixelsHorizontal;

			// Tile index in the ValidityFlags array
			const int TileArrayIdx = CurCol * TilesVertical + CurRow;

			// Fake tiles produce Nan values
			if (TilesValidityFlags[TileArrayIdx] == false)
			{
				PFMDataOut.Add(NanVector);
				continue;
			}

			// Pixel offset in tile space
			const FVector TileSpaceOffset = FVector(0.f, MarginX + TilePixelX * PixelOffsetX, -(MarginY + TilePixelY * PixelOffsetY));
			// Pixel in world space
			const FVector WorldSpacePixel = CurTileTransform.TransformPosition(TileSpaceOffset);
			// Pixel in the PFM origin space
			const FVector PFMSpacePixel = PFMOrigin->GetActorTransform().InverseTransformPosition(WorldSpacePixel);

			// Store current pixel data
			PFMDataOut.Add(PFMSpacePixel);
		}
	}

}


