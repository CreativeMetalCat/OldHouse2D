// Fill out your copyright notice in the Description page of Project Settings.


#include "SpriteHoldableActor.h"

ASpriteHoldableActor::ASpriteHoldableActor()
{
    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    RootComponent = Sprite;
    Sprite->SetSimulatePhysics(true);
    Sprite->SetConstraintMode(EDOFMode::XZPlane);

    ColliderBox->SetupAttachment(RootComponent);
}

void ASpriteHoldableActor::BeHeld_Implementation(AActor* HoldingActor)
{
    SetActorEnableCollision(false);
    Holder = HoldingActor;
    Sprite->SetSimulatePhysics(false);
    if(PickupSound!=nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickupSound,GetActorLocation(),GetActorRotation());
    }
}

void ASpriteHoldableActor::BeDropped_Implementation(AActor* HoldingActor)
{
    SetActorEnableCollision(true);
    Holder = nullptr;
    Sprite->SetSimulatePhysics(true);
    if(PickupSound!=nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickupSound,GetActorLocation(),GetActorRotation());
    }
}
