// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "HSTypes.h"
#include "BTDecorator_HSTeamIs.generated.h"

/**
 * 
 */



UCLASS()
class HIDESEEKWGHW7_API UBTDecorator_HSTeamIs : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ETeamType TeamType;
	
	/** calculates raw, core value of decorator's condition. Should not include calling IsInversed */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
