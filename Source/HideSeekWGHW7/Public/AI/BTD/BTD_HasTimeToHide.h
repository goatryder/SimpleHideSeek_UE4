// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_HasTimeToHide.generated.h"

/**
 * 
 */
UCLASS()
class HIDESEEKWGHW7_API UBTD_HasTimeToHide : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float TimeIsShortVal;

	/** calculates raw, core value of decorator's condition. Should not include calling IsInversed */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
