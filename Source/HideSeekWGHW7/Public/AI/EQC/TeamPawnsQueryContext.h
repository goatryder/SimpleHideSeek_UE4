// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "TeamPawnsQueryContext.generated.h"

/**
 * 
 */
UCLASS()
class HIDESEEKWGHW7_API UTeamPawnsQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()
	
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const;

};
