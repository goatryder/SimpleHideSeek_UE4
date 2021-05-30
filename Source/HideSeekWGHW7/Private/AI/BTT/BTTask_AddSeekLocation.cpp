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
	for (auto& Pos : AICon->GetCheckedPositions())
	{
		DrawDebugSphere(GetWorld(), Pos, 30.0f, 12, FColor(0.25f, 1.f, 1.f), false, 1.0f);
	}

	FString Msg = FString::Printf(TEXT("Total %d"), AICon->GetCheckedPositions().Num());
	DrawDebugString(GetWorld(), Loc, Msg, 0, FColor::White, 4.0f, true);

	DrawDebugSphere(GetWorld(), Loc, 70.0f, 12, FColor::Cyan, false, 2.0f);

	if (Loc != FVector::ZeroVector)
	{
		AICon->AddCheckedPosition(Loc);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}