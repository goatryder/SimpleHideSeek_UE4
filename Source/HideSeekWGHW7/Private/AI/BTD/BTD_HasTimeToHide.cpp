// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTD/BTD_HasTimeToHide.h"
#include "HSGameMode.h"
#include "GameFramework/GameStateBase.h"

bool UBTD_HasTimeToHide::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (AHSGameMode* GM = Cast<AHSGameMode>(GetWorld()->GetGameState()->AuthorityGameMode))
	{
		if (GM->GetHiddingStageTimeLeft() > TimeIsShortVal)
		{
			return true;
		}
	}

	return false;
}
