// Fill out your copyright notice in the Description page of Project Settings.


#include "HSGameMode.h"

#include "HSHUD.h"
#include "HSCharacterPlayer.h"
#include "Components/HSTeamComponent.h"
#include "../HideSeekWGHW7.h"

AHSGameMode::AHSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_HideAndSeek/BP_CharacterPlayer"));
	if (PlayerPawnClassFinder.Succeeded())
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	else
		DefaultPawnClass = AHSCharacterPlayer::StaticClass();

	// use our custom Player Controller class
	PlayerControllerClass = AHSHUD::StaticClass();

	// use our custom HUD class
	static ConstructorHelpers::FClassFinder<AHUD> HUDClassFinder(TEXT("/Game/_HideAndSeek/BP_HUD"));
	if (HUDClassFinder.Succeeded())
		HUDClass = HUDClassFinder.Class;
	else
		HUDClass = AHSHUD::StaticClass();
	
	SetGameStage(EHSGameStage::Preparation);

	HiddingStageTime = 10.0f;

	UHSTeamComponent::NotifyTeamCompTeamChanged.AddUObject(this, &AHSGameMode::OnTeamChangeEvent);
}

void AHSGameMode::StartPlay()
{
	Super::StartPlay();
	HandleHiddingStage();
}

void AHSGameMode::SetGameStage(EHSGameStage NewGameStage)
{
	GameStage = NewGameStage;

	// Debug
	if (GEngine)
	{
		FString MsgStage = TEXT("New GameMode Stage: ") + EnumToStringLocal(TEXT("EHSGameStage"), static_cast<uint8>(GameStage));
		GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Yellow, MsgStage);
	}
}

void AHSGameMode::HandleHiddingStage()
{
	SetGameStage(EHSGameStage::Hidding);
	GetWorldTimerManager().SetTimer(TimerHandle_SeekStageCountDown, this, &AHSGameMode::HandleSeekingStage, HiddingStageTime, false, 0.0);
}

void AHSGameMode::HandleSeekingStage()
{
	SetGameStage(EHSGameStage::Seeking);
}

void AHSGameMode::CheckGameOver()
{
	bool bIsGameOver = false;

	if (bIsGameOver)
	{
		HandleGameOverStage();
	}
}

void AHSGameMode::HandleGameOverStage()
{
	SetGameStage(EHSGameStage::Over);

	RestartGame();
}

void AHSGameMode::OnTeamChangeEvent(UHSTeamComponent* TeamComp, AHSCharacter* Character, ETeamType NewTeam)
{
	CheckGameOver();
}
