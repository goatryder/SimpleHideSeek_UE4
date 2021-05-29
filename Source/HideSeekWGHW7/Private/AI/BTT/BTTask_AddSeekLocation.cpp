// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT/BTTask_AddSeekLocation.h"
#include "AI/HSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

EBTNodeResult::Type UBTTask_AddSeekLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AHSAIController* AICon = Cast<AHSAIController>(OwnerComp.GetAIOwner());
	if (AICon == NULL)
	{
		return EBTNodeResult::Failed;
	}

	FVector Loc = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

	if (Loc != FVector::ZeroVector)
	{
		AICon->AddCheckedPosition(Loc);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}