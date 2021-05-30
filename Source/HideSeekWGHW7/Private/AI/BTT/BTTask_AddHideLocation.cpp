// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT/BTTask_AddHideLocation.h"
#include "AI/HSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#include "DrawDebugHelpers.h"

EBTNodeResult::Type UBTTask_AddHideLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AHSAIController* AICon = Cast<AHSAIController>(OwnerComp.GetAIOwner());
	if (AICon == NULL)
	{
		return EBTNodeResult::Failed;
	}

	FVector Loc = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

	// Debug
	for (auto& Pos : AICon->GetChosenHideLocations())
	{
		DrawDebugSphere(GetWorld(), Pos, 30.0f, 12, FColor(1.0f, 0.25f, 0.25f), false, 3.0f);
	}

	FString Msg = FString::Printf(TEXT("Total %d"), AICon->GetChosenHideLocations().Num());
	DrawDebugString(GetWorld(), Loc, Msg, 0, FColor::White, 4.0f, true);

	DrawDebugSphere(GetWorld(), Loc, 70.0f, 12, FColor::Red, false, 4.0f);

	if (Loc != FVector::ZeroVector)
	{
		AICon->AddChosenHideLocation(Loc);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}