// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HSCharacterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HIDESEEKWGHW7_API AHSCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

	AHSCharacterPlayerController();

	UFUNCTION()
		virtual void SetupInputComponent() override;

protected:

	void SetPlayerCharacter(class APawn* Pawn);
	class AHSCharacterPlayer* PlayerCharacter;

	void Jump();
	void MoveForward(float Val);
	void MoveRight(float Val);
	void Turn(float Val);
	void LookUp(float Val);

};
