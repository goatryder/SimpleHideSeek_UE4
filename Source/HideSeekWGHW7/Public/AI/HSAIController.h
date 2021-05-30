// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "HSTypes.h"
#include "HSAIController.generated.h"

/**
 * 
 */

UCLASS()
class HIDESEEKWGHW7_API AHSAIController : public AAIController
{
	GENERATED_BODY()

public:

	AHSAIController();

	// Gets called when the perception component updates. This is
	// where check to see if the PlayerCharacter was detected.
	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedActors);

protected:

	void BeginPlay() override;

	// A reference to the hearing perception component.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI)
		class UBehaviorTree* BTAsset;
	
	class AHSCharacterAI* OwnedCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI)
		float PeripheralVisionAngleDegrees;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI)
		FName BBKey_GameStage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI)
		FName BBKey_PreyActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI)
		FName BBKey_HunterActor;

	// Is Seeker character already spot hiding character?
	static bool HasHunterActor(AHSAIController* AICon);
	// Is Hiding character already has seeker character?
	static bool HasPreyActor(AHSAIController* AICon);
	
	// Set BB HunterActor Value
	static void SetHunterActor(AHSAIController* AICon, AActor* HunterActor);
	// Set BB PreyActor Value
	static void SetPreyActor(AHSAIController* AICon, AActor* PrayActor);

	// Called when the AIController is taken over.
	virtual void OnPossess(APawn* PossesedPawn) override;
	
	// already checked points where characters can possibly hide;
	static TArray<FVector> CheckedPositions;

public:

	// Change blackboard instance synched value "GameStage"
	static void HandleGameStageChanged(class AHSGameMode* GM, EHSGameStage NewGameStage);

	static TArray<FVector> &GetCheckedPositions() { return CheckedPositions; }

	static void AddCheckedPosition(FVector Position) { CheckedPositions.Add(Position); }

	static void ClearCheckedPositions() { CheckedPositions.Empty(); }
};
