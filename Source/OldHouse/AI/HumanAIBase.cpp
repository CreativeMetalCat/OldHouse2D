// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanAIBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "OldHouse/OldHouseCharacter.h"
#include "Engine.h"

AHumanAIBase::AHumanAIBase()
{
    Senses = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Senses"));

    bSetControlRotationFromPawnOrientation = false;
}

ETeamAttitude::Type AHumanAIBase::GetTeamAttitudeTowards(const AActor& Other) const
{
    GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,Other.GetName());
    //for now system only cares about old house characters
    if(Cast<AOldHouseCharacter>(&Other)!=nullptr)
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
    
}

void AHumanAIBase::UpdatedSeenActors(const TArray<AActor*>& actors)
{
    if (GetBlackboardComponent() != nullptr)
    {
        bool bSeenEnemy = false;
        bool bCanSeeEnemy = false;
        if (GetBlackboardComponent()->GetValueAsObject(FName("Target")) != nullptr)
        {
            bSeenEnemy = true;
            bCanSeeEnemy = (actors.Find(Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FName("Target")))) != -1);
        }
        if (actors.Num() > 0 && !bCanSeeEnemy)
        {
            GetBlackboardComponent()->SetValueAsObject(FName("Target"),actors[0]);
            
            for (int i = 0; i < actors.Num(); i++)
            {
                
            }
        }
    }
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
}
