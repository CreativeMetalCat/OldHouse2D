// Fill out your copyright notice in the Description page of Project Settings.


#include "KnifeBase.h"

AKnifeBase::AKnifeBase()
{
    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    RootComponent = Sprite;

    DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
    DamageBox->SetupAttachment(RootComponent);
    
    DamageBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    
    //this can be easily changed by it's children
    //but for now knife is only supposed to damage people
    DamageBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AKnifeBase::DealDamage()
{
    TArray<AActor*> actors;
    DamageBox->GetOverlappingActors(actors);
    if (actors.Num() > 0)
    {
        for (int i = 0; i < actors.Num(); i++)
        {
            if (actors[i] != WeaponOwner)
            {
                actors[i]->TakeDamage(Damage, FPointDamageEvent(), nullptr, WeaponOwner);
            }
        }
    }
}
