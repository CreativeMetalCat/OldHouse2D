// Fill out your copyright notice in the Description page of Project Settings.


#include "SpriteDoor.h"

ASpriteDoor::ASpriteDoor()
{
    SpriteClosedState = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteClosedState"));

    SpriteOpenedState = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT(" SpriteOpenedState"));

    SpriteClosedState->SetVisibility(!bOpen);
    SpriteOpenedState->SetVisibility(bOpen);
	
    SpriteClosedState->SetupAttachment((RootComponent));
    SpriteOpenedState->SetupAttachment((RootComponent));
}

void ASpriteDoor::OnToggle()
{
    SpriteClosedState->SetVisibility(!bOpen);
    SpriteClosedState->SetCollisionEnabled(bOpen?ECollisionEnabled::NoCollision:ECollisionEnabled::QueryOnly);
    SpriteOpenedState->SetVisibility(bOpen);
    SpriteOpenedState->SetCollisionEnabled(!bOpen?ECollisionEnabled::NoCollision:ECollisionEnabled::QueryOnly);
}
