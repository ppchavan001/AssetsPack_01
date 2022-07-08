// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapToSequencerCameraActor.h"
#include "Camera/CameraComponent.h"
#include "EditorLevelLibrary.h"
#include "Misc/EngineVersionComparison.h"

#if UE_VERSION_NEWER_THAN(4, 27, 2)

#include "Subsystems/EditorActorSubsystem.h"

#endif


#include "EngineUtils.h"

// Sets default values
ASnapToSequencerCameraActor::ASnapToSequencerCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(USceneComponent::GetDefaultSceneRootVariableName());
	RootComponent->Mobility = EComponentMobility::Movable;

	this->SetRootComponent(RootComponent);

	RootComponent->RegisterComponent();

}

// Called when the game starts or when spawned
void ASnapToSequencerCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnapToSequencerCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SequenceToFollow)
	{
		SetActorTransform(SequenceToFollow->SequencePlayer->GetActiveCameraComponent()->GetComponentTransform());

		if (!SequenceToFollow->SequencePlayer->IsPlaying()) SequenceToFollow = NULL;

	}
	else
	{
	#if UE_VERSION_NEWER_THAN(4, 27, 2)
		UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();

		TArray<class AActor*> AllActorsInCurrentWorld = EditorActorSubsystem ? EditorActorSubsystem->GetAllLevelActors() : TArray<AActor*>();

	#else
		TArray<class AActor*> AllActorsInCurrentWorld = UEditorLevelLibrary::GetAllLevelActors();

	#endif

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Searching for new Sequence actor to follow."));


		for (TActorIterator<AActor> It(this->GetWorld()); It; ++It)
		{
			if (ALevelSequenceActor * SeqActor = Cast<ALevelSequenceActor>(*It))
			{

				if (SeqActor->SequencePlayer->IsPlaying()) SequenceToFollow = SeqActor;
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Following New Sequence actor %s"));
			}
		}
	
	}

}

