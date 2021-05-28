// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
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
		void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

protected:

	void BeginPlay() override;

	// A reference to the hearing perception component.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;
	
	class AHSCharacterAI* OwnedCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float PeripheralVisionAngleDegrees;

	// Called when the AIController is taken over.
	virtual void OnPossess(APawn* PossesedPawn) override;
	
	// already checked points where characters can possibly hide;
	static TArray<FVector> CheckedPositions;

public:

	UFUNCTION(BlueprintCallable)
		static TArray<FVector> GetCheckedPositions() { return CheckedPositions; }

	UFUNCTION(BlueprintCallable)
		static void AddCheckedPosition(FVector Position) { CheckedPositions.Add(Position); }

	UFUNCTION(BlueprintCallable)
		static void ClearCheckedPositions() { CheckedPositions.Empty(); }
};
