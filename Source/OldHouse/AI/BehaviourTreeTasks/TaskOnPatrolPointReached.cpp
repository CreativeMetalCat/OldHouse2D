// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskOnPatrolPointReached.h"
#include  "OldHouse/AI/HumanAIBAse.h"

EBTNodeResult::Type UTaskOnPatrolPointReached::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
   if(OwnerComp.GetAIOwner() != nullptr)
   {
       AHumanAIBase* ai = Cast<AHumanAIBase>(OwnerComp.GetAIOwner());
       if(ai != nullptr)
       {
           ai->SelectNewPatrollingPoint();
       }
   }
    return EBTNodeResult::Type::Succeeded;
}
