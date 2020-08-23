// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolBase.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"


APistolBase::APistolBase()
{
    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    RootComponent = Sprite;

    AnimType = EWeaponAnimType::EWT_Pistol;

    CooldownTime = 1.f;
}

bool APistolBase::Fire(FVector Location,FRotator Rotaion)
{
    if(CanShoot())
    {
        if (GetWorld() != nullptr)
        {
            ABulletBase* bullet = GetWorld()->SpawnActor<ABulletBase>(BulletClass, Location, Rotaion);
            if (bullet != nullptr)
            {
          
                bullet->WeaponOwner = WeaponOwner;
                //shot by player
                if(WeaponOwner->Tags.Find("Player")!=-1)
                {
                    bullet->Tags.Add("ShotByPlayer");
                }
                else
                {
                    bullet->Tags.Add("ShotByEnemy");
                }
                bullet->SetOwner(WeaponOwner);
                bullet->Collision->MoveIgnoreActors.Add(WeaponOwner);                
            }
            else
            {
                return false;
            }

            if (FireSound != nullptr)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, Location, Rotaion);
            }
            StartCooldownTimer();
            return true;
        }
    }
    return false;
}
