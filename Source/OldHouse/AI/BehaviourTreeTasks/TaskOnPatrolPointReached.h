// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskOnPatrolPointReached.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API UTaskOnPatrolPointReached : public UBTTaskNode
{
	GENERATED_BODY()
	public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
