// Fill out your copyright notice in the Description page of Project Settings.


#include "HSAIController.h"
#include "HSCharacter.h"
#include "HSCharacterAI.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/HSTeamComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "../HideSeekWGHW7.h"

TArray<FVector> AHSAIController::CheckedPositions;

AHSAIController::AHSAIController()
{
	// Create the hearing sense, and a AIPerceptionComponent and 
	// set it as the default perception component of the AIController.
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Hearing Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	
	// detect all
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PeripheralVisionAngleDegrees = 45.0f;

	// Set sight as the dominant sense.
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	// default bb key names
	BBKey_PreyActor = FName(TEXT("PreyActor"));
	BBKey_HunterActor = FName(TEXT("HunterActor"));
}

void AHSAIController::BeginPlay()
{
	AAIController::BeginPlay();
	
	// set vision angle
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
	
	// observe pawn detection
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AHSAIController::OnPawnDetected);

}

void AHSAIController::OnPawnDetected(const TArray<AActor*>& DetectedActors)
{
	if (!OwnedCharacter)
	{
		return;
	}

	ETeamType ConPawnTeamType = OwnedCharacter->TeamComp->GetTeam();
	
	// AI Controlled Pawn is Seeker case
	if (ConPawnTeamType == ETeamType::Seek && !HasPreyActor(this))
	{
		for (AActor* DetectedActor : DetectedActors)
		{
			UHSTeamComponent* TeamComp = Cast<UHSTeamComponent>(DetectedActor->GetComponentByClass(UHSTeamComponent::StaticClass()));
			
			if (TeamComp && TeamComp->GetTeam() == ETeamType::Hide)
			{

				// try to set hunter actor for hidding pawn
				if (APawn* DetectedPawn = Cast<APawn>(DetectedActor))
				{
					AHSAIController* AICon = Cast<AHSAIController>(DetectedPawn->GetController());

					if (!HasHunterActor(AICon))
					{
						SetHunterActor(AICon, OwnedCharacter);
					}
				}

				// try to set pray actor for seeker pawn
				SetPreyActor(this, DetectedActor);
				break;
			}

			//// Debug
			//AHSCharacter* DetectedCharacter = Cast<AHSCharacter>(DetectedPawn);
			//if (DetectedCharacter && GEngine)
			//{
			//	FString MsgLocation = FString::Printf(TEXT("Detected Character %s at Loc: %s "), *DetectedCharacter->GetName(), *DetectedCharacter->GetActorLocation().ToString());
			//	ETeamType Team = DetectedCharacter->TeamComp->GetTeam();
			//	FString MsgTeam = TEXT("Team: ") + EnumToStringLocal(TEXT("ETeamType"), static_cast<uint8>(Team));

			//	GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, MsgLocation + MsgTeam);
			//}
		}
	}
	else if (ConPawnTeamType == ETeamType::Hide)
	{

	}
}

void AHSAIController::OnPossess(APawn* PossesedPawn)
{
	Super::OnPossess(PossesedPawn);

	OwnedCharacter = Cast<AHSCharacterAI>(PossesedPawn);
}

bool AHSAIController::HasHunterActor(AHSAIController* AICon)
{
	if (AICon)
	{
		UObject* HunterActor = AICon->GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(AICon->BBKey_HunterActor);
		
		return HunterActor != nullptr;
	}
	
	return false;
}

bool AHSAIController::HasPreyActor(AHSAIController* AICon)
{
	if (AICon)
	{
		UObject* PreyActor = AICon->GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(AICon->BBKey_PreyActor);
		
		return PreyActor != nullptr;
	}

	return false;
}

void AHSAIController::SetHunterActor(AHSAIController* AICon, AActor* HunterActor)
{
	if (AICon)
	{
		AICon->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(AICon->BBKey_HunterActor, HunterActor);
	}
}

void AHSAIController::SetPreyActor(AHSAIController* AICon, AActor* PreyActor)
{
	if (AICon)
	{
		AICon->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(AICon->BBKey_PreyActor, PreyActor);
	}
}