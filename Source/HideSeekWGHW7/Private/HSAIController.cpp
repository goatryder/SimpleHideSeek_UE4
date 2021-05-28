// Fill out your copyright notice in the Description page of Project Settings.


#include "HSAIController.h"
#include "HSCharacter.h"
#include "HSCharacterAI.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/HSTeamComponent.h"

TArray<FVector> AHSAIController::CheckedPositions;

AHSAIController::AHSAIController()
{
	// Create the hearing sense, and a AIPerceptionComponent and 
	// set it as the default perception component of the AIController.
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Hearing Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	// Set the hearing sense to detect everything. This should be changed to fit your needs
	// but it makes it easiest for the tutorial.
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PeripheralVisionAngleDegrees = 45.0f;

	// Set sight as the dominant sense.
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AHSAIController::BeginPlay()
{
	AAIController::BeginPlay();
	
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AHSAIController::OnPawnDetected);

}

void AHSAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	// todo: if seeker finds hider, then make it seeker
	for (AActor* DetectedPawn : DetectedPawns)
	{
		AHSCharacter* DetectedCharacter = Cast<AHSCharacter>(DetectedPawn);
		
		if (DetectedCharacter)
		{
			// DEBUG
			if (GEngine)
			{
				bool bIsTeamRed = DetectedCharacter->TeamComp->TeamIsRed();

				GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green,
					FString::Printf(TEXT("Detected Character At Location: %s Team Red: %d"), *DetectedCharacter->GetActorLocation().ToString(), bIsTeamRed));
			}
		}
	}
}

void AHSAIController::OnPossess(APawn* PossesedPawn)
{
	Super::OnPossess(PossesedPawn);

	OwnedCharacter = Cast<AHSCharacterAI>(PossesedPawn);
}