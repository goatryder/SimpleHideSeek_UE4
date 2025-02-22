// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/HSAIController.h"
#include "AI/HSCharacterAI.h"
#include "HSCharacter.h"
#include "Components/HSTeamComponent.h"
#include "../HideSeekWGHW7.h"
#include "HSGameMode.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

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
	BBKey_GameStage = FName(TEXT("GameStage"));

	// try to find BT Asset
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> FoundBT(TEXT("/Game/_HideAndSeek/HeyHi/BT_AIHideSeek.BT_AIHideSeek"));
	if (FoundBT.Succeeded())
	{
		BTAsset = FoundBT.Object;
	}	
}

void AHSAIController::BeginPlay()
{
	AAIController::BeginPlay();
	
	// set vision angle
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
	
	// observe pawn detection
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AHSAIController::OnPawnDetected);

	if (BTAsset)
	{
		RunBehaviorTree(BTAsset);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Please select BTAsset for HSAIController instance"));
	}
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

void AHSAIController::HandleGameStageChanged(AHSGameMode* GM, EHSGameStage NewGameStage)
{
	// Change Blackboard value GameState
	for (FConstControllerIterator Iterator = GM->GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		if (AController* Con = Iterator->Get())
		{
			if (AHSAIController* AICon = Cast<AHSAIController>(Con))
			{
				// Debug
				if (GEngine)
				{
					uint32 BBGameStageNum = AICon->GetBlackboardComponent()->GetValue<UBlackboardKeyType_Int>(AICon->BBKey_GameStage);

					FString Msg = FString::Printf(TEXT("[HSAIController] BB GameStage Old: %d"), BBGameStageNum);
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, Msg);
				}

				int GameStageNum = static_cast<uint8>(NewGameStage);
				AICon->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Int>(AICon->BBKey_GameStage, GameStageNum);

				// Debug
				if (GEngine)
				{
					uint32 BBGameStageNum = AICon->GetBlackboardComponent()->GetValue<UBlackboardKeyType_Int>(AICon->BBKey_GameStage);

					FString Msg = FString::Printf(TEXT("[HSAIController] BB GameStage New: %d"), BBGameStageNum);
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, Msg);
				}

				break;
			}
		}
	}

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