// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretAIBase.h"
#include "OldHouse/OldHouseCharacter.h"
#include "Perception/AISense_Sight.h"

ATurretAIBase::ATurretAIBase()
{
    Senses = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Senses"));
    
    PrimaryActorTick.bCanEverTick = true;
}

void ATurretAIBase::BeginPlay()
{
    if(GetPawn() != nullptr)
    {
        Turret = Cast<ATurretBase>(GetPawn());
    }
    GetWorldTimerManager().SetTimer(UpdateTimerHandle,this,&ATurretAIBase::UpdatePerceivedActors,1.f,true);
}

void ATurretAIBase::UpdatePerceivedActors()
{
    TArray<AActor*> actors;

    Senses->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), actors);

    if (Turret != nullptr)
    {
        if(Turret->CurrentTarget != nullptr)
        {
            AOldHouseCharacter*human = Cast<AOldHouseCharacter>(Turret->CurrentTarget);
            if(human != nullptr)
            {
               bCanSeeTarget = (actors.Find(Cast<AActor>(Turret->CurrentTarget)) != INDEX_NONE) && human->CanBeSeen();
            }
            else
            {
                bCanSeeTarget = (actors.Find(Cast<AActor>(Turret->CurrentTarget)) != INDEX_NONE);
            }
            if(!bCanSeeTarget)
            {
                Turret->CurrentTarget = nullptr;
            }
        }
        if(!bCanSeeTarget && actors.Num() > 0)
        {
            for (int i = 0; i < actors.Num(); i++)
            {
                if(actors[i]->Tags.Find(Turret->TargetTag) != -1)
                {
                    AOldHouseCharacter*human = Cast<AOldHouseCharacter>(actors[i]);
                    if(human != nullptr)
                    {
                        if(human->CanBeSeen())
                        {
                            Turret->CurrentTarget = actors[i];
                            bCanSeeTarget = true;
                            break;
                        }
                    }
                    else
                    {
                        Turret->CurrentTarget = actors[i];
                        bCanSeeTarget = true;
                        break;
                    }                
                }
            }
        }
    }
    else if(GetPawn() != nullptr)//this is just to make sure we have proper data
    {
        Turret = Cast<ATurretBase>(GetPawn());
    }
}

void ATurretAIBase::Tick(float DeltaSeconds)
{
    if(bCanSeeTarget && Turret != nullptr)
    {     
        Turret->Shoot();
    }
}

ETeamAttitude::Type ATurretAIBase::GetTeamAttitudeTowards(const AActor& Other) const
{
    if(Turret != nullptr)
    {
        if (Other.Tags.Find(Turret->TargetTag) != -1)
        {
            return ETeamAttitude::Type::Hostile;
        }
        else
        {
            return ETeamAttitude::Type::Neutral;
        }
    }
    else
    {
        return ETeamAttitude::Type::Neutral;
    }
}
