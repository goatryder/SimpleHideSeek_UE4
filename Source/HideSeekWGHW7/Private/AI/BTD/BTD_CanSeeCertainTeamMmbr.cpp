// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTD/BTD_CanSeeCertainTeamMmbr.h"
#include "AI/HSAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/HSTeamComponent.h"

bool UBTD_CanSeeCertainTeamMmbr::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AHSAIController* AICon = Cast<AHSAIController>(OwnerComp.GetOwner());
	if (AICon)
	{
		UAIPerceptionComponent* Perception = AICon->GetPerceptionComponent();

		if (Perception)
		{
			TArray<AActor*> PercievedActors;
			Perception->GetPerceivedActors(UAISense_Sight::StaticClass(), PercievedActors);

			for (auto& Actor : PercievedActors)
			{
				UHSTeamComponent* TeamComp = Cast<UHSTeamComponent>(Actor->GetComponentByClass(UHSTeamComponent::StaticClass()));
				if (TeamComp && TeamComp->GetTeam() == TeamType)
				{
					return true;
				}
			}
		}
	}

	return false;
}
