// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQC/SeekTeamPawnsQueryContext.h"
#include "Components/HSTeamComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "HSTypes.h"

void USeekTeamPawnsQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);
	
	AActor* QueryActor = Cast<AActor>((QueryInstance.Owner).Get());

	if (QueryActor)
	{
		UHSTeamComponent* TeamComp = Cast<UHSTeamComponent>(QueryActor->GetComponentByClass(UHSTeamComponent::StaticClass()));
		
		if (TeamComp)
		{
			TArray<AActor*> ActorSet;

			TArray<UHSTeamComponent*> TeamComps = UHSTeamComponent::GetTeamComponents(ETeamType::Seek);

			for (auto& Elem : TeamComps)
			{
				if (Elem && Elem->GetOwner())
				{
					ActorSet.Add(Elem->GetOwner());
				}
			}

			UEnvQueryItemType_Actor::SetContextHelper(ContextData, ActorSet);
		}
	}
}
