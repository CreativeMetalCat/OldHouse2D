// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimatedHoldableActor.h"

AAnimatedHoldableActor::AAnimatedHoldableActor()
{
    PhysicsBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PhysicsCollider"));
    PhysicsBox->SetCollisionResponseToAllChannels(::ECR_Block);
    PhysicsBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    PhysicsBox->SetConstraintMode(EDOFMode::XZPlane);

    RootComponent = PhysicsBox;

    Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
    Sprite->SetupAttachment(RootComponent);


    PhysicsBox->SetSimulatePhysics(true);

    ColliderBox->SetupAttachment(RootComponent);
}

void AAnimatedHoldableActor::BeHeld_Implementation(AActor* HoldingActor)
{
    SetActorEnableCollision(false);
    Holder = HoldingActor;
    PhysicsBox->SetSimulatePhysics(false);
    if(PickupSound!=nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickupSound,GetActorLocation(),GetActorRotation());
    }
}

void AAnimatedHoldableActor::BeDropped_Implementation(AActor* HoldingActor)
{
    SetActorEnableCollision(true);
    Holder = nullptr;
    PhysicsBox->SetSimulatePhysics(true);
    if(PickupSound!=nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickupSound,GetActorLocation(),GetActorRotation());
    }
}
