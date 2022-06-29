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

FKey UExposeRuntimeFunctionsBPLibrary::GetKeyFromName(FName name)
{
	return FKey(name);

}

void UExposeRuntimeFunctionsBPLibrary::SetFPropertyByName(UObject* Object, FName NameOfThePropertyToUpdate, const FString DataToSet)
{

	if (Object)
	{
		UClass* _Class = Object->GetClass();

		FProperty* property = _Class->FindPropertyByName(NameOfThePropertyToUpdate);

		SetFPropertyValueInternal(property, Object, DataToSet);
	}
}


FString UExposeRuntimeFunctionsBPLibrary::BufferToString(const TArray<uint8>& DataBuffer)
{

	if (DataBuffer[DataBuffer.Num() - 1] == 0x00)
	{
		return UTF8_TO_TCHAR(DataBuffer.GetData());
	}

	TArray<uint8> tempBuffer;
	tempBuffer.SetNum(DataBuffer.Num() + 1);
	FMemory::Memcpy(tempBuffer.GetData(), DataBuffer.GetData(), DataBuffer.Num());
	tempBuffer[tempBuffer.Num() - 1] = 0x00;

	return UTF8_TO_TCHAR(tempBuffer.GetData());
}


void UExposeRuntimeFunctionsBPLibrary::StringToBuffer(const FString& message, TArray<uint8>& DataBuffer)
{
	std::string _str = TCHAR_TO_UTF8(*message);

	DataBuffer.SetNum(_str.size() + 1);
	DataBuffer[DataBuffer.Num() - 1] = 0x00;

	FMemory::Memcpy(DataBuffer.GetData(), _str.c_str(), _str.size());

}

void UExposeRuntimeFunctionsBPLibrary::ConvertStringToVector(TArray<FString> Lines, TArray<FVector>& VerticesOut)
{
	VerticesOut.Empty();

	for (auto line : Lines)
	{
		FVector Vec;
		// X=-0.000 Y=0.496 Z=-0.476
		TArray<FString> arr;
		line.ParseIntoArray(arr, &FString(" ")[0]);

		FString val;
		arr[0].Split("=", nullptr, &val);
		Vec.X = FCString::Atof(&val[0]);


		arr[0].Split("=", nullptr, &val);
		Vec.X = FCString::Atof(&val[0]);


		arr[1].Split("=", nullptr, &val);
		Vec.Y = FCString::Atof(&val[0]);


		arr[2].Split("=", nullptr, &val);
		Vec.Z = FCString::Atof(&val[0]);


		VerticesOut.Add(Vec);

	}

}





FString UExposeRuntimeFunctionsBPLibrary::ConvertVectorArrayToString(TArray<FString>& LinesOut, TArray<FVector> Vertices)
{
	FString StringOut;

	LinesOut.Empty();
	for (auto vert : Vertices)
	{
		auto vertStr = vert.ToString();
		LinesOut.Add(vertStr);
		StringOut += vertStr;
		StringOut += "\n";
	}

	return StringOut;
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
TArray<FVector> UExposeRuntimeFunctionsBPLibrary::AddDeltaToMatrixVertices(const TArray<FVector>& VerticesIn, const int32 TargetChannel, const float MaxDelta, const int32 MatrixWidth, bool InvertDelta)
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

		auto Delta = FMath::Lerp(MaxDelta, (float)0, FinalAlphaAngle);

		if (InvertDelta) Delta = 1 - Delta;

		auto vec = VerticesIn[i];

		if (i < MatrixWidth) UE_LOG(LogTemp, Warning, TEXT("Delta : %f"), Delta);
		
		switch (TargetChannel)
		{
			case 0:
				vec.X += Delta;
				break;

			case 1:
				vec.Y += Delta;
				break;

			case 2:
				vec.Z += Delta;
				break;
		}

		VecArr.Add(vec);


	}





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

void UExposeRuntimeFunctionsBPLibrary::SetFPropertyValueInternal(FProperty* property, void* InContainer, const FString DataToSet)
{

#define Object InContainer

	// If property is valid for the object
	// Determine property type
	if (property && Object)
	{



		// *************************************************
		// 
		// BoolProperty		: valid values -> "true" or "false"
		// 
		// *************************************************

		if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(property))
		{
			FString DataTrimmed = DataToSet.TrimStartAndEnd();

			bool bValue = DataTrimmed.Compare("true", ESearchCase::IgnoreCase) == 0;
			BoolProperty->SetPropertyValue(BoolProperty->ContainerPtrToValuePtr< void >(Object), bValue);

			return;
		}



		// *************************************************
		// 
		// NameProperty	: valid values ->  any string will be converted to name
		// 
		// *************************************************

		if (FNameProperty* NameProperty = CastField<FNameProperty>(property))
		{
			FString DataTrimmed = DataToSet.TrimStartAndEnd();

			FName Value = FName(*DataTrimmed);
			NameProperty->SetPropertyValue(NameProperty->ContainerPtrToValuePtr< void >(Object), Value);

			return;
		}



		// *************************************************
		// 
		// StrProperty	: valid values ->  any string
		// 
		// *************************************************

		if (FStrProperty* StrProperty = CastField<FStrProperty>(property))
		{
			StrProperty->SetPropertyValue(StrProperty->ContainerPtrToValuePtr< void >(Object), DataToSet);

			return;
		}





		// *************************************************
		// 
		// TextProperty		: valid values -> Any string will be converted to text
		// 
		// *************************************************
		if (FTextProperty* TextProperty = CastField<FTextProperty>(property))
		{
			FText Value = FText::FromString(DataToSet);
			TextProperty->SetPropertyValue(TextProperty->ContainerPtrToValuePtr< void >(Object), Value);

			return;
		}




		// *************************************************
		// 
		// EnumProperty		: valid values -> 0 to (255 or max Enum size which ever is lower), 
		// 
		// *************************************************
		if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(property))
		{
			FNumericProperty* UnderlyingProperty = EnumProperty->GetUnderlyingProperty();

			if (UnderlyingProperty)
			{
				auto PropertyValuePtr = (EnumProperty->ContainerPtrToValuePtr< void >(Object));
				FString DataString = DataToSet.TrimStartAndEnd();

				if (DataString.Len() == 0) DataString = "0";

				UnderlyingProperty->SetNumericPropertyValueFromString(PropertyValuePtr, &(DataString[0]));

			}

			return;
		}




		// *************************************************
		// 
		// Array Property
		// 
		// Array of int, float, bool, etc
		// : valid values -> Comma separated values of the supported type
		// 
		// *************************************************
		if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(property))
		{

			// Build CSV array
			TArray<FString> DataArray;
			DataToSet.ParseIntoArray(DataArray, *FString(","), false);


			FScriptArrayHelper_InContainer Helper(ArrayProperty, InContainer);

			// Clear old values
			Helper.EmptyValues(DataArray.Num());

			// Add empty values for assignment
			Helper.AddValues(DataArray.Num());



			// Assign string values from Data Array to Array property
			volatile auto x = Helper.Num();

			for (int32 DynamicIndex = 0; DynamicIndex < Helper.Num(); ++DynamicIndex)
			{
				void* ValuePtr = Helper.GetRawPtr(DynamicIndex);


				// Data Cleaning and validation
				FString Data = DataArray[DynamicIndex];
				Data.TrimStartAndEndInline();
				if (Data.Len() < 1)	Data = "0";

				SetFPropertyValueInternal(ArrayProperty->Inner, ValuePtr, Data);

			}

			return;
		}



		// *************************************************
		// 
		// Numeric Property 
		// int, float, double
		// 
		// *************************************************

		if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(property))
		{
			auto PropertyValuePtr = (NumericProperty->ContainerPtrToValuePtr< void >(Object));
			FString DataString = DataToSet.TrimStartAndEnd();

			if (DataString.Len() == 0) DataString = "0";
			NumericProperty->SetNumericPropertyValueFromString(PropertyValuePtr, &(DataString[0]));

			return;
		}




		// *************************************************
		// 
		// FStruct Property 
		// vector, color, transform
		// : 
		//	Color					: comma separated keys ex. R:0, G: 25, B:127, A:255 or CSV in same order
		//	Vector(location, scale)	: comma separated keys ex. LocX: 125, LocY: 778, LocZ : -220 or CSV in same order
		//	Rotator					: comma separated keys ex. RotX : -75, RotY: 76, RotZ: -650 or CSV in same order
		//	Transform				: comma separated keys ex. LocX: 400,  RotZ: -100,   ScaleX: 12.2, ScaleY : -54, ScaleZ: 70 
		//								or CSV in Loc(x,y,z), Rotation(x,y,z), Scale(x,y,z)
		//
		// 
		// *************************************************


		FStructProperty* StructProperty = CastField<FStructProperty>(property);
		if (StructProperty)
		{
			// ---------------------
			// Setup Start
			// ---------------------

			FString StuctTypeName = FString(StructProperty->Struct->GetName());


			// Get CSV data from string
			TArray<FString> CSVArray;
			DataToSet.ParseIntoArray(CSVArray, *FString(","), false);


			// Separate channel data by ":" from CSV data
			TMap<FString, FString> ChannelData;
			for (FString Data : CSVArray)
			{
				FString Key;
				FString Value;

				Data.Split(":", &Key, &Value);

				Key.TrimStartAndEndInline();
				Value.TrimStartAndEndInline();


				// If key is present then add it as a channel
				if (Key.Len() > 0)	ChannelData.Add(Key, Value);

			}



		/*-----------------------------------------------------------
		|															|
		|	Key Definitions	to lookup in loaded strings				|
		|															|
		-------------------------------------------------------------*/



		// Update this value defending on the default value of the channel 
		// ex Location = "0", scale = "1"
		#define DefaultChannelValue "This value should never be used! Replace with default value for the channel"

		// Return FString for specified channel
		// If key is defined 
		//		return value for key
		// else 
		//	if index is valid
		//		return Value at index 
		//	else 
		//		return default value for channel
		#define AssignChannelValue(Key, Index) (ChannelData.Contains(Key) && ChannelData[Key].Len() > 0) ? ChannelData[Key] : ((CSVArray.Num() > Index && !(CSVArray[Index].Contains(":"))) ? CSVArray[Index] : DefaultChannelValue)


		#define ColorR	"R"
		#define ColorG 	"G"
		#define ColorB 	"B"
		#define ColorA 	"A"

		#define LocationX "LocX"
		#define LocationY "LocY"
		#define LocationZ "LocZ"


		#define RotationX "RotX"
		#define RotationY "RotY"
		#define RotationZ "RotZ"
		#define RotationW "RotW"

		#define ScaleX "ScaleX"
		#define ScaleY "ScaleY"
		#define ScaleZ "ScaleZ"

		#define ImplementLocation(LocationXIndex, LocationYIndex, LocationZIndex) \
			FVector Location;\
			Location.X = FCString::Atof(&(AssignChannelValue(LocationX, LocationXIndex))[0]);\
			Location.Y = FCString::Atof(&(AssignChannelValue(LocationY, LocationYIndex))[0]);\
			Location.Z = FCString::Atof(&(AssignChannelValue(LocationZ, LocationZIndex))[0]);

		#define ImplementRotator(RotationXIndex, RotationYIndex, RotationZIndex, RotationWIndex) \
			FQuat Rotation;\
			Rotation.X = FCString::Atof(&(AssignChannelValue(RotationX, RotationXIndex))[0]);\
			Rotation.Y = FCString::Atof(&(AssignChannelValue(RotationY, RotationYIndex))[0]);\
			Rotation.Z = FCString::Atof(&(AssignChannelValue(RotationZ, RotationZIndex))[0]);\
			Rotation.W = FCString::Atof(&(AssignChannelValue(RotationW, RotationWIndex))[0]);


		//Reset warning for macros with multiple values
		#pragma warning (default: 4003) 

			// ---------------------
			// Setup End
			// ---------------------


			// Vector i.e location, scale, etc
			if (StuctTypeName == "Vector")
			{
			#define DefaultChannelValue "0"

				ImplementLocation(0, 1, 2);

				StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr< void >(Object), &Location);

				return;

			}



			// Rotation
			if (StuctTypeName == "Rotator")
			{
			#define DefaultChannelValue "0"

				ImplementRotator(0, 1, 2, 3);

				StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr< void >(Object), &Rotation);

				return;

			}


			// Color(0-255) Struct
			if (StuctTypeName == "Color")
			{

			#define DefaultChannelValue "255"


				FColor ColorValue;
				ColorValue.R = FCString::Atoi(&(AssignChannelValue(ColorR, 0))[0]);
				ColorValue.G = FCString::Atoi(&(AssignChannelValue(ColorG, 1))[0]);
				ColorValue.B = FCString::Atoi(&(AssignChannelValue(ColorB, 2))[0]);
				ColorValue.A = FCString::Atoi(&(AssignChannelValue(ColorA, 3))[0]);

				StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr< void >(Object), &ColorValue);

				return;

			}

			// Transform
			if (StuctTypeName == "Transform")
			{

			#define DefaultChannelValue "0"

				ImplementLocation(0, 1, 2);
				ImplementRotator(3, 4, 5, 9);



			#define DefaultChannelValue "1"

				FVector Scale;
				Scale.X = FCString::Atof(&(AssignChannelValue(ScaleX, 6))[0]);
				Scale.Y = FCString::Atof(&(AssignChannelValue(ScaleY, 7))[0]);
				Scale.Z = FCString::Atof(&(AssignChannelValue(ScaleZ, 8))[0]);



				FTransform Transform;
				Transform.SetComponents(Rotation, Location, Scale);

				StructProperty->CopyCompleteValue(StructProperty->ContainerPtrToValuePtr< void >(Object), &Transform);

				return;
			}

			return;
		}


	}
}


FString UExposeRuntimeFunctionsBPLibrary::GetFPropertyClassName(UObject* Object, FName PropertyName)
{
	if (Object)
	{
		UClass* _Class = Object->GetClass();

		FProperty* property = _Class->FindPropertyByName(PropertyName);

		if (property) return property->GetClass()->GetName();

		return FString("Invalid Property name!");

	}
	return FString("Invalid Object!");
}



