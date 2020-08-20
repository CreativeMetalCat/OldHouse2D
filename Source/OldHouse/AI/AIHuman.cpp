// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHuman.h"
#include "PaperFlipbookComponent.h"


void AAIHuman::OnUnPosses()
{
    GetSprite()->SetFlipbook(UnPossesAnimation);
    GetSprite()->SetLooping(false);
    bDead = true;
}

bool AAIHuman::CanBePossesed()
{
    return !bDead;
}
