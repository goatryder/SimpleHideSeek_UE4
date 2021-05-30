// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQC/HidingPosesQueryContext.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "AI/HSAIController.h"

#include "DrawDebugHelpers.h"

void UHidingPosesQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	//Get the Owner of this Query and cast it to an actor
	//Then, get the actor's controller and cast to it our AIController
	//This code works for our case but avoid that many casts and one-liners in cpp.
	AHSAIController* AICon = Cast<AHSAIController>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));

	if (AICon)
	{
		TArray<FVector> ChosenHideLocations = AICon->GetChosenHideLocations();

		//Set the context SeeingPawn to the provided context data
		UEnvQueryItemType_Point::SetContextHelper(ContextData, ChosenHideLocations);

		/*// Debug
		for (FVector& Pos : ChosenHideLocations)
		{
			DrawDebugSphere(GetWorld(), Pos, 50.0f, 12, FColor::Red, false, 4.0f);
		}*/
	}
}
