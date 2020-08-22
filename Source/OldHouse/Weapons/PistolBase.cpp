// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolBase.h"

APistolBase::APistolBase()
{
    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    RootComponent = Sprite;

    AnimType = EWeaponAnimType::EWT_Pistol;
}
