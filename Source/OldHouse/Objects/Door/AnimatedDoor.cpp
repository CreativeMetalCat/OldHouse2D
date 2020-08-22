// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimatedDoor.h"

AAnimatedDoor::AAnimatedDoor()
{
    Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
    Sprite->SetupAttachment(RootComponent);
}

void AAnimatedDoor::OnToggle()
{
    if (Sprite != nullptr && MoveAnim != nullptr)
    {
        Sprite->SetLooping(false);
        Sprite->SetFlipbook(MoveAnim);
        bIsMoving = true;
        GetWorldTimerManager().SetTimer(StopMoveAnimTimerHandle, this, &AAnimatedDoor::OnMoveAnimStopped,Sprite->GetFlipbookLength());
        if (bOpen)
        {
            Sprite->PlayFromStart();
        }
        else
        {
            Sprite->ReverseFromEnd();
        }
    }
}

void AAnimatedDoor::OnMoveAnimStopped()
{
    bIsMoving = false;
    if(!bOpen)
    {
        Sprite->SetLooping(true);
        Sprite->SetFlipbook(ClosedAnim);
    }
}

bool AAnimatedDoor::CanBeToggled()
{
    return !bIsMoving;
}
