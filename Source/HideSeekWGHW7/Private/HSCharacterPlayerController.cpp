// Fill out your copyright notice in the Description page of Project Settings.


#include "HSCharacterPlayerController.h"
#include "HSCharacterPlayer.h"

AHSCharacterPlayerController::AHSCharacterPlayerController()
{
	
	OnNewPawn.AddUObject(this, &AHSCharacterPlayerController::SetPlayerCharacter);
}

void AHSCharacterPlayerController::SetupInputComponent()
{
	APlayerController::SetupInputComponent();

	InputComponent->BindAxis("MoveRight", this, &AHSCharacterPlayerController::MoveRight);
	InputComponent->BindAxis("MoveForward", this, &AHSCharacterPlayerController::MoveForward);
	InputComponent->BindAxis("Turn", this, &AHSCharacterPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AHSCharacterPlayerController::LookUp);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AHSCharacterPlayerController::Jump);
}

void AHSCharacterPlayerController::SetPlayerCharacter(APawn* PossesedPawn)
{
	AHSCharacterPlayer* PossesedCharacter = Cast<AHSCharacterPlayer>(PossesedPawn);
	if (PossesedCharacter)
	{
		PlayerCharacter = PossesedCharacter;
	}
}

void AHSCharacterPlayerController::Jump()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->Jump();
	}
}

void AHSCharacterPlayerController::MoveForward(float Val)
{
	if (PlayerCharacter)
	{
		FVector FwdVec = GetControlRotation().Vector();
		FwdVec.Z = 0.0f;
		FwdVec.Normalize();

		PlayerCharacter->AddMovementInput(FwdVec * Val);
	}
}

void AHSCharacterPlayerController::MoveRight(float Val)
{
	if (PlayerCharacter)
	{
		// todo: fix rotation
		PlayerCharacter->AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y) * Val);
	}
}

void AHSCharacterPlayerController::Turn(float Val)
{
	if (PlayerCharacter)
	{
		AddYawInput(Val);
	}
}

void AHSCharacterPlayerController::LookUp(float Val)
{
	if (PlayerCharacter)
	{
		AddPitchInput(Val);
	}
}
