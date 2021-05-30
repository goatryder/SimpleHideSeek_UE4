// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HSTypes.h"
#include "HSGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGameStageChange, AHSGameMode*, EHSGameStage);

/**
 * 
 */
UCLASS()
class HIDESEEKWGHW7_API AHSGameMode : public AGameMode
{
	GENERATED_BODY()

	AHSGameMode();
	virtual void BeginDestroy() override;

	virtual void StartPlay() override;

protected:

	UPROPERTY(BlueprintReadOnly)
		EHSGameStage GameStage;

	void SetGameStage(EHSGameStage NewGameStage);

	void HandleHiddingStage();
	void HandleSeekingStage();
	void CheckGameOver();
	void HandleGameOverStage();

	FTimerHandle TimerHandle_SeekStageCountDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HideAndSeek")
		float HiddingStageTime;

public:

	EHSGameStage GetGameStage() const { return GameStage; }

	static FOnGameStageChange NotifyOnGameStage;

	UFUNCTION()
		void OnTeamChangeEvent(class UHSTeamComponent* TeamComp, class AHSCharacter* Character, ETeamType NewTeam);
};
