// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HSCharacter.h"
#include "HSCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class HIDESEEKWGHW7_API AHSCharacterPlayer : public AHSCharacter
{
	GENERATED_BODY()

public:
	AHSCharacterPlayer();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		class UCameraComponent* CameraComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		class USpringArmComponent* SpringArmComp;

};
