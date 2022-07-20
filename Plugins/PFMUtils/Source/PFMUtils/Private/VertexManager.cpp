// Fill out your copyright notice in the Description page of Project Settings.


#include "VertexManager.h"
#include "PFMUtilsBPLibrary.h"

// Sets default values
AVertexManager::AVertexManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVertexManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVertexManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AVertexManager::LoadVertices(FString FileName)
{
	TArray<FString> Lines = UPFMUtilsBPLibrary::ReadLinesFromFile(FileName);

	UPFMUtilsBPLibrary::ConvertStringToVector(Lines, Vertices);

	return true;
}

