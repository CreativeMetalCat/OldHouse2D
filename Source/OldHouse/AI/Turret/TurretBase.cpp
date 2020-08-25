// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretBase.h"

// Sets default values
ATurretBase::ATurretBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurretBaseSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TurretBaseSprite"));
    RootComponent = TurretBaseSprite;

	TurretGunSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("TurretGunSprite"));
	TurretGunSprite->SetupAttachment(RootComponent,TEXT("GunSocket"));
}

// Called when the game starts or when spawned
void ATurretBase::BeginPlay()
{
	Super::BeginPlay();
	Weapon = GetWorld()->SpawnActor<APistolBase>(WeaponClass);
	if (Weapon != nullptr)
	{
		Weapon->AttachToComponent(TurretGunSprite, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MuzzleSocket"));
		Weapon->WeaponOwner = this;
	}
}

// Called every frame
void ATurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TurretGunSprite->AddLocalRotation(FRotator(DeltaTime * Speed * bReachedRight ? -1 : 1, 0, 0));
	if (TurretGunSprite->GetRelativeRotation().Pitch >= MaxRotationAngle) { bReachedRight = true; }
	else if (TurretGunSprite->GetRelativeRotation().Pitch <= MaxRotationAngle * -1) { bReachedRight = false; }
	Shoot();
}

void ATurretBase::Shoot()
{
	if(!ResetAnimTimerHandle.IsValid())
	{
		if(Weapon != nullptr)
		{
			Weapon->Fire(TurretGunSprite->GetSocketLocation(TEXT("MuzzleSocket")),GetShootingRotation());
			if(ShootingAnim != nullptr)
			{
				TurretGunSprite->SetLooping(false);
				TurretGunSprite->SetFlipbook(ShootingAnim);
				TurretGunSprite->PlayFromStart();
				GetWorldTimerManager().SetTimer(ResetAnimTimerHandle,this,&ATurretBase::ResetAnimation,TurretGunSprite->GetFlipbookLength(),false);
			}
		}
	}
}

void ATurretBase::ResetAnimation()
{
	if(IdleAnim != nullptr)
	{
		TurretGunSprite->SetFlipbook(IdleAnim);
		TurretGunSprite->SetLooping(true);
		TurretGunSprite->PlayFromStart();
		GetWorldTimerManager().ClearTimer(ResetAnimTimerHandle);
	}
}

FRotator ATurretBase::GetShootingRotation()
{
	FRotator res = TurretGunSprite->GetSocketRotation(TEXT("MuzzleSocket"));
	res.Pitch += 180.f;
	return res;
}

