// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "HSTypes.h"
#include "BTD_CanSeeCertainTeamMmbr.generated.h"

/**
 * 
 */

UCLASS()
class HIDESEEKWGHW7_API UBTD_CanSeeCertainTeamMmbr : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ETeamType TeamType;

	/** calculates raw, core value of decorator's condition. Should not include calling IsInversed */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
