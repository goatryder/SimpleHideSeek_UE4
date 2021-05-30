// Fill out your copyright notice in the Description page of Project Settings.


#include "HSGameMode.h"

#include "HSHUD.h"
#include "HSCharacterPlayer.h"
#include "HSCharacterPlayerController.h"
#include "Components/HSTeamComponent.h"
#include "AI/HSAIController.h"
#include "../HideSeekWGHW7.h"

FOnGameStageChange AHSGameMode::NotifyOnGameStage;

AHSGameMode::AHSGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_HideAndSeek/BP_CharacterPlayer"));
	if (PlayerPawnClassFinder.Succeeded())
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	else
		DefaultPawnClass = AHSCharacterPlayer::StaticClass();

	// use our custom Player Controller class
	PlayerControllerClass = AHSCharacterPlayerController::StaticClass();

	// use our custom HUD class
	static ConstructorHelpers::FClassFinder<AHUD> HUDClassFinder(TEXT("/Game/_HideAndSeek/BP_HUD"));
	if (HUDClassFinder.Succeeded())
		HUDClass = HUDClassFinder.Class;
	else
		HUDClass = AHSHUD::StaticClass();
	
	SetGameStage(EHSGameStage::Preparation);

	HiddingStageTime = 10.0f;
}

void AHSGameMode::BeginDestroy()
{
	Super::BeginDestroy();

	// unsubsribe from delegate
	NotifyOnGameStage.Clear();
}

void AHSGameMode::StartPlay()
{
	AHSAIController::ClearCheckedPositions();

	Super::StartPlay();

	// subscribe to delegate to change blackboard instance synched value "GameStage"
	NotifyOnGameStage.AddStatic(AHSAIController::HandleGameStageChanged);

	// subscribe to TeamComp OnTeamChange
	UHSTeamComponent::NotifyTeamCompTeamChanged.AddUObject(this, &AHSGameMode::OnTeamChangeEvent);

	HandleHiddingStage();
}

void AHSGameMode::Tick(float DeltaTime)
{
	// Hidding Stage Count Down Timer
	if (bHiddingStageTimeCntDwnActive)
	{
		HiddingStageTimeLeft -= DeltaTime;

		if (HiddingStageTimeLeft < 0.0f)
		{
			HiddingStageTimeLeft = 0.0f;
			bHiddingStageTimeCntDwnActive = false;
		}
	}
}

void AHSGameMode::SetGameStage(EHSGameStage NewGameStage)
{
	GameStage = NewGameStage;

	NotifyOnGameStage.Broadcast(NewGameStage);

	/*// Debug
	if (GEngine)
	{
		FString MsgStage = TEXT("New GameMode Stage: ") + EnumToStringLocal(TEXT("EHSGameStage"), static_cast<uint8>(GameStage));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, MsgStage);
	}*/
}

void AHSGameMode::HandleHiddingStage()
{
	SetGameStage(EHSGameStage::Hidding);

	ResetHiddingStageTimeCntDwn();

	GetWorldTimerManager().SetTimer(TimerHandle_SeekStageCountDown, this, &AHSGameMode::HandleSeekingStage, HiddingStageTime);
}

void AHSGameMode::HandleSeekingStage()
{
	SetGameStage(EHSGameStage::Seeking);
}

void AHSGameMode::CheckGameOver()
{
	int32 HiddingTeamNum = UHSTeamComponent::GetTeamComponents(ETeamType::Hide).Num();

	// Debug
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("[HSGameMode] OnTeamChange HiddingTeamNum: %d"), HiddingTeamNum);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, Msg);
	}

	// No one is in Hide Team, Means all Hidding characters is found
	if (HiddingTeamNum == 0)
	{
		HandleGameOverStage();
	}
}

void AHSGameMode::HandleGameOverStage()
{
	SetGameStage(EHSGameStage::Over);
	//ResetLevel();
}

void AHSGameMode::OnTeamChangeEvent(UHSTeamComponent* TeamComp, AHSCharacter* Character, ETeamType NewTeam)
{
	CheckGameOver();
}
