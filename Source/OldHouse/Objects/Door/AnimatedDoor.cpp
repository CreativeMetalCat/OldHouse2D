// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimatedDoor.h"

AAnimatedDoor::AAnimatedDoor()
{
    Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
}

void AAnimatedDoor::Toggle_Implementation()
{
    Super::Toggle();

    if(bOpen)
    {
        Sprite->SetLooping(false);
        Sprite->SetFlipbook(MoveAnim);
        Sprite->PlayFromStart();
        GetWorldTimerManager().SetTimer(StopMoveAnimTimerHandle,this,&AAnimatedDoor::OnMoveAnimStopped,Sprite->GetFlipbookLength());
    }
    else
    {
        Sprite->SetLooping(false);
        Sprite->SetFlipbook(MoveAnim);
        Sprite->ReverseFromEnd();
        GetWorldTimerManager().SetTimer(StopMoveAnimTimerHandle,this,&AAnimatedDoor::OnMoveAnimStopped,Sprite->GetFlipbookLength());
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
