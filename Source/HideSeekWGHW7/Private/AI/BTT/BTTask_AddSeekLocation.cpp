// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT/BTTask_AddSeekLocation.h"
#include "AI/HSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#include "DrawDebugHelpers.h"

EBTNodeResult::Type UBTTask_AddSeekLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AHSAIController* AICon = Cast<AHSAIController>(OwnerComp.GetAIOwner());
	if (AICon == NULL)
	{
		return EBTNodeResult::Failed;
	}

	FVector Loc = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

	// Debug
	DrawDebugString(GetWorld(), Loc + FVector(0.0f, 0.0f, 50.0f), TEXT("New Seek Location"), 0, FColor::White, 3.0f);
	DrawDebugSphere(GetWorld(), Loc, 50.0f, 12, FColor::Cyan, false, 3.0f);
	for (auto& CheckedLoc : AHSAIController::GetCheckedPositions())
	{
		DrawDebugSphere(GetWorld(), CheckedLoc, 25.0f, 12, FColor::Cyan, false, 3.0f);
	}

	if (Loc != FVector::ZeroVector)
	{
		AICon->AddCheckedPosition(Loc);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}