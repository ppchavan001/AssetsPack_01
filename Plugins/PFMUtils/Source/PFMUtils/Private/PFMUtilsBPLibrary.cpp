// Copyright Epic Games, Inc. All Rights Reserved.

#include "PFMUtilsBPLibrary.h"
#include "PFMUtils.h"
#include "DrawDebugHelpers.h"
#include "Misc/FileHelper.h"

UPFMUtilsBPLibrary::UPFMUtilsBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void UPFMUtilsBPLibrary::ConvertStringToVector(TArray<FString> Lines, TArray<FVector>& VerticesOut, const bool IsMatrix)
{
	VerticesOut.Empty();

	if (IsMatrix)
	{

		TArray<FString> XArr;
		Lines[0].ParseIntoArray(XArr, &FString(",")[0]);


		TArray<FString> YArr;
		Lines[1].ParseIntoArray(YArr, &FString(",")[0]);

		TArray<FString> ZArr;
		Lines[2].ParseIntoArray(ZArr, &FString(",")[0]);

		for (auto Z : ZArr)
			for (auto Y : YArr)
				for (auto X : XArr)
				{
					FVector vec;
					vec.X = FCString::Atof(&X[0]);
					vec.Y = FCString::Atof(&Y[0]);
					vec.Z = FCString::Atof(&Z[0]);

					VerticesOut.Add(vec);
				}

		
		return;

	}

	for (auto line : Lines)
	{
		FVector Vec;


		// OLD X=-0.000 Y=0.496 Z=-0.476
		// New CSV  0,3,5.4

		TArray<FString> arr;
		line.ParseIntoArray(arr, &FString(",")[0], false);


		const FString DefaultChannelVal = "0.0";

	#define GetFloatFromStr(ArrIndex) FCString::Atof(arr[ArrIndex].Len() > 0 ? &arr[ArrIndex][0] : &DefaultChannelVal[0])

		Vec.X = GetFloatFromStr(0);
		Vec.Y = GetFloatFromStr(1);
		Vec.Z = GetFloatFromStr(2);


		VerticesOut.Add(Vec);

	}
	


}


#pragma optimize( "", on )
FString UPFMUtilsBPLibrary::ConvertVectorArrayToString(TArray<FString>& LinesOut, TArray<FVector> Vertices)
{
	FString StringOut;

	LinesOut.Empty();
	for (auto vert : Vertices)
	{
		// start a new line for new vertex
		if (StringOut.Len() > 0) StringOut += "\n";

		FString vertStr;

		if (vert.X != 0.0) vertStr += FString::SanitizeFloat(vert.X);
		vertStr += ',';
		if (vert.Y != 0.0) vertStr += FString::SanitizeFloat(vert.Y);
		vertStr += ',';
		if (vert.Z != 0.0) vertStr += FString::SanitizeFloat(vert.Z);
		

		LinesOut.Add(vertStr);

		StringOut += vertStr;

	}

	return StringOut;
}


FString UPFMUtilsBPLibrary::ConvertMatrixArrayToString(const TArray<FVector> Vertices)
{
	FString StringOut;

	float MaxX = -999999999;
    float MaxY = -999999999;
	float MaxZ = -999999999;
	for (auto vert : Vertices)
	{
          if (vert.X > MaxX) MaxX = vert.X;
          if (vert.Y > MaxY) MaxY = vert.Y;
          if (vert.Z > MaxZ) MaxZ = vert.Z;
	}
	
	FString XVals;
	FString YVals;
	FString ZVals;

	for (auto vert : Vertices)
	{
          if (vert.Y == MaxY && vert.Z == MaxZ) 
		  {
			  if (XVals.Len() > 0) XVals += ",";
			  XVals += FString::SanitizeFloat(vert.X);
		  }


		  if (vert.X == MaxX && vert.Z == MaxZ)
		  {
			  if (YVals.Len() > 0) YVals += ",";
			  YVals += FString::SanitizeFloat(vert.Y);
		  }


		  if (vert.Y == MaxY && vert.X == MaxX)
		  {
			  if (ZVals.Len() > 0) ZVals += ",";
			  ZVals += FString::SanitizeFloat(vert.Z);
		  }

	}
	StringOut = XVals + '\n' + YVals + '\n' + ZVals;
	return StringOut;

}



#pragma optimize( "", on )
void UPFMUtilsBPLibrary::MassDebugDrawPoint(const TArray<FVector>& Vertices, 
											const AActor* ParentActor, 
											const float DrawPercentage /*= 25*/, 
											const FVector DeltaLocation /*= FVector(0,0,0)*/, 
											float Size /*= 1.0f*/,
											FColor const& Color /*= FColor(255,255,0,255)*/,
											bool bPersistentLines /*= false*/, 
											float LifeTime /*= 2.0f*/, 
											const bool UseSlotsToDraw /*= false */,
											uint8 DepthPriority /*= 0*/)
{
	if (!ParentActor || !ParentActor->GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("ExposedEditorPlugin [MassDebugDrawPoint]: Invalid parent actor or world!"));
		return;
	}

	if(UseSlotsToDraw)
	{
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
	else
	{
		int i = 0;
		while (i < Vertices.Num())
		{
			if (FMath::RandRange(0, 100) < DrawPercentage)
			{
				auto Position = Vertices[i];
				DrawDebugPoint(ParentActor->GetWorld(), Position + DeltaLocation, Size, Color, bPersistentLines, LifeTime, DepthPriority);
			}
			++i;
		}
	}
}


#pragma optimize("", on)
TArray<FVector> UPFMUtilsBPLibrary::AddDeltaToMatrixVertices(const TArray<FVector>& VerticesIn,
															 const FDeltaVerticesRequiredData DeltaVerticesData
															 )
{
	/* ********************
		Data prep
	******************** */

	TArray<FVector> VecArr;

	int32 ModifiedVertex = 0;

	float MinD = 9999999999;
	float MaxD = -9999999999;
	float AvgD = 0;


	// Input Start And End Row
	auto _StartRow = DeltaVerticesData.StartRow;
	auto _EndRow = DeltaVerticesData.EndRow;

	// Processed start and End row
	// Always processed from lower value to the higher
	auto StartRow = _StartRow < _EndRow ? _StartRow : _EndRow;
	auto EndRow = _StartRow > _EndRow ? _StartRow : _EndRow;

	// Input Start And End Columns
	auto _StartColumn = DeltaVerticesData.StartColumn;
	auto _EndColumn = DeltaVerticesData.EndColumn;

	// Processed start and End Columns
	// Always processed from lower value to the higher
	auto StartColumn = _StartColumn < _EndColumn ? _StartColumn : _EndColumn;
	auto EndColumn = _StartColumn > _EndColumn ? _StartColumn : _EndColumn;




	/* ****************************************
		Process per vertex
	**************************************** */

	for (int32 i = 0; i < VerticesIn.Num(); ++i)
	{
		// get column number of this vertex
		auto ColumnNumber = i % DeltaVerticesData.MatrixWidth;
		
		// get row number of this vertex
		auto  RowNumber = FMath::DivideAndRoundDown(i , DeltaVerticesData.MatrixWidth);


		// Range Check
		// Only modify if in the given range
		// Processed start/ end numbers for simplicity in operations
		if (RowNumber >= StartRow && RowNumber < EndRow
			&& ColumnNumber >= StartColumn && ColumnNumber < EndColumn)
		{
			/* ********************
				Determining Alpha/ falloff of the vertex
			******************** */

			// Converts the column number to 0 - 1 range
			auto AlphaX = (ColumnNumber - StartColumn ) * DeltaVerticesData.FadeFalloffRow / (float)(EndColumn - StartColumn);

			// Converts the row number to 0 - 1 range
			auto AlphaY = (RowNumber - StartRow) * DeltaVerticesData.FadeFalloffColumn / (float)(EndRow - StartRow);

			// If start number is greater than end number,
			// the user expects an inverted effect,
			// hence the alpha is inverted for the respective type
			if (_StartColumn > _EndColumn) AlphaX = 1 - AlphaX;
			if (_StartRow > _EndRow) AlphaY = 1 - AlphaY;



			/* ********************
				Smoothing and curving, Alpha/ falloff of the vertex
			******************** */


			// combining horizontal and vertical alpha / falloff
			auto Alpha = FMath::Clamp(FMath::Sqrt(AlphaX * AlphaX + AlphaY * AlphaY), 0.0f, 1.0f);

			// converting alpha to a range 0 - 90 to prepare for sin (0 - 1) conversion
			auto AlphaAngle = Alpha * 90;

			// converting range to smooth curve of range 0 - 1
			auto FinalAlphaAngle = FMath::Sin(PI / (180.f) * AlphaAngle);



			/* ********************
				Converting alpha into delta for the vertex
			******************** */


			// lerp range 0 - MaxDelta by FinalAlphaAngle
			auto Delta = FMath::Lerp(DeltaVerticesData.MaxDelta, 0.0f, FinalAlphaAngle);

			if (DeltaVerticesData.InvertDelta) Delta = 1 - Delta;

			auto vec = VerticesIn[i];

			


			/* ********************
				Add delta to vertex
			******************** */


			// Switch based on the channel we are trying to modify
			switch (DeltaVerticesData.TargetChannel)
			{
			case EAxis3D::X:
				vec.X += Delta;
				break;

			case EAxis3D::Y:
				vec.Y += Delta;
				break;

			case EAxis3D::Z:
				vec.Z += Delta;
				break;
			}

			VecArr.Add(vec);
			
			
			/* ********************
				Counters
			******************** */

			++ModifiedVertex;

			if (Delta < MinD) MinD = Delta;
			if (Delta > MaxD) MaxD = Delta;
			AvgD += Delta;
		}
		else
		{
			VecArr.Add(VerticesIn[i]);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Total modified vertex count : %d"), ModifiedVertex);
	UE_LOG(LogTemp, Warning, TEXT("Min delta : %f"), MinD);
	UE_LOG(LogTemp, Warning, TEXT("Max delta : %f"), MaxD);
	UE_LOG(LogTemp, Warning, TEXT("Avg delta : %f"), AvgD / ModifiedVertex);
	return VecArr;
}


#pragma optimize("", on)
void UPFMUtilsBPLibrary::GeneratePFMDataOnly(const FString& File, const FVector& StartLocation, const FRotator& StartRotation, const AActor* PFMOrigin, const int TilesHorizontal, const int TilesVertical, const float ColumnAngle, const float TileSizeHorizontal, const float TileSizeVertical, const int TilePixelsHorizontal, const int TilePixelsVertical, const bool AddMargin, const TArray<bool>& TilesValidityFlags, TArray<FVector>& PFMDataOut)
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

bool UPFMUtilsBPLibrary::WriteStringToFile(const FString FileName, const FString DataToWrite)
{
	return FFileHelper::SaveStringToFile(DataToWrite, &FileName[0]);

}

bool UPFMUtilsBPLibrary::ReadLinesFromFile(const FString FileName, TArray<FString>& LinesOut)
{
	return FFileHelper::LoadFileToStringArray(LinesOut, &FileName[0]);
}

bool UPFMUtilsBPLibrary::WriteVerticesToFile(const FString FileName, const TArray<FVector> Vertices, const bool MatrixOptimization)
{
	TArray<FString> Lines;
	FString Data;
	
	if (MatrixOptimization)
	{
		Data = ConvertMatrixArrayToString(Vertices);
	}
	else
	{
		Data = ConvertVectorArrayToString(Lines, Vertices);
	}
	return WriteStringToFile(FileName, Data);
}


