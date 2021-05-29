// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTD/BTDecorator_HSTeamIs.h"
#include "Components/HSTeamComponent.h"

bool UBTDecorator_HSTeamIs::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActor* QueryOwner = OwnerComp.GetOwner();
	AController* ControllerOwner = Cast<AController>(QueryOwner);
	if (ControllerOwner)
	{
		QueryOwner = ControllerOwner->GetPawn();
		
		if (QueryOwner)
		{
			UHSTeamComponent* TeamComp = Cast<UHSTeamComponent>(QueryOwner->GetComponentByClass(UHSTeamComponent::StaticClass()));

			if (TeamComp && TeamComp->GetTeam() == TeamType)
			{
				return true;
			}
		}
	}

	return false;
}
