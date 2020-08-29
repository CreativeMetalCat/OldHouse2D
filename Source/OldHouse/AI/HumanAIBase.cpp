// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanAIBase.h"

#include "AIHuman.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "OldHouse/OldHouseCharacter.h"

AHumanAIBase::AHumanAIBase()
{
    Senses = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Senses"));

    bSetControlRotationFromPawnOrientation = false;
}

ETeamAttitude::Type AHumanAIBase::GetTeamAttitudeTowards(const AActor& Other) const
{
    if (Other.Tags.Find("Player") != -1)
    {
        return ETeamAttitude::Type::Hostile;
    }
    else
    {
        return ETeamAttitude::Type::Neutral;
    }
}

void AHumanAIBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    
    UpdatePerceivedActors();
    
    if (bCanSeeEnemy)
    {
        if (GetPawn() != nullptr)
        {
            AOldHouseCharacter* human = Cast<AOldHouseCharacter>(GetPawn());
            if (human != nullptr)
            {
                human->Attack();
            }
        }
    }
}

void AHumanAIBase::UpdatedSeenActors(const TArray<AActor*>& actors)
{
    
}

void AHumanAIBase::BeginPlay()
{
    Super::BeginPlay();
    if (Senses != nullptr)
    {
        PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AHumanAIBase::UpdatedSeenActors);
    }

    if(BehaviorTree!=nullptr)
    {
        RunBehaviorTree(BehaviorTree);
    }

    InitPatrollingSystem();
    
    GetWorldTimerManager().SetTimer(UpdateTimerHandle,this,&AHumanAIBase::UpdatePerceivedActors,1.f,true);
}

void AHumanAIBase::UpdatePerceivedActors()
{
    TArray<AActor*>actors;
    Senses->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(),actors);
    if (GetBlackboardComponent() != nullptr)
    {
        bool bSeenEnemy = false;
        if (GetBlackboardComponent()->GetValueAsObject(FName("Target")) != nullptr)
        {
            bSeenEnemy = true;
            AOldHouseCharacter*human = Cast<AOldHouseCharacter>(GetBlackboardComponent()->GetValueAsObject(FName("Target")));
            if(human != nullptr)
            {
                bCanSeeEnemy = (actors.Find(Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FName("Target")))) != INDEX_NONE) && human->CanBeSeen();
            }
            else
            {
                bCanSeeEnemy = (actors.Find(Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FName("Target")))) != INDEX_NONE);
            }
            if(!bCanSeeEnemy)
            {
                GetBlackboardComponent()->ClearValue(FName("Target"));
            }
        }
        if (actors.Num() > 0 && !bCanSeeEnemy)
        {
            for (int i = 0; i < actors.Num(); i++)
            {
                if(actors[i]->Tags.Find("Player") != -1)
                {
                    AOldHouseCharacter*human = Cast<AOldHouseCharacter>(actors[i]);
                    if(human != nullptr)
                    {
                        if(human->CanBeSeen())
                        {
                            GetBlackboardComponent()->SetValueAsObject(FName("Target"), actors[i]);
                            bCanSeeEnemy = true;
                            return;
                        }
                    }
                    else
                    {
                        GetBlackboardComponent()->SetValueAsObject(FName("Target"), actors[i]);
                        bCanSeeEnemy = true;
                        return;
                    }
                }
            }
        }
        
    }
}

void AHumanAIBase::InitPatrollingSystem()
{
    CurrentPatrollingSystem = GetPatrollingSystem();
    
    if(CurrentPatrollingSystem!= nullptr)
    {
        SetNewPatrolPoint();
    }
}

bool AHumanAIBase::SetNewPatrolPoint()
{
    if( GetBlackboardComponent() != nullptr && CurrentPatrollingSystem != nullptr)
    {
        if(CurrentPatrollingSystem->Points.IsValidIndex(CurrentPatrolPointId))
        {         
            GetBlackboardComponent()->SetValueAsObject(FName("MovementTarget"), CurrentPatrollingSystem->Points[CurrentPatrolPointId]);            
            return true;
        }
    }
    return false;
}

int32 AHumanAIBase::SelectNewPatrollingPoint()
{
    if(CurrentPatrollingSystem!= nullptr)
    {
        if(CurrentPatrollingSystem->Points.IsValidIndex(CurrentPatrolPointId+1))
        {
            CurrentPatrolPointId += 1;
        }
        else
        {
            CurrentPatrolPointId = 0;
        }
        SetNewPatrolPoint();
        return CurrentPatrolPointId;
    }
    else
    {
        return INDEX_NONE;
    }
}

APatrolSystemBase* AHumanAIBase::GetPatrollingSystem()
{
    if (GetPawn() != nullptr)
    {
      
        AAIHuman * human = Cast<AAIHuman>(GetPawn());
        if(human != nullptr)
        {
            return human->CurrentPatrollingSystem;
        }
    }
    
    return nullptr;
}

APatrollingPointBase* AHumanAIBase::GetCurrentPatrollingPoint()
{
    if(CurrentPatrollingSystem!= nullptr)
    {
        if(CurrentPatrollingSystem->Points.IsValidIndex(CurrentPatrolPointId))
        {
            return CurrentPatrollingSystem->Points[CurrentPatrolPointId];
        }
    }
    return nullptr;
}
