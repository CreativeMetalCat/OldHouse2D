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

	//in case usage of ai perception fail -  uncomment this code and work from here
	
    /*ViewBoxCenter = CreateDefaultSubobject<UBoxComponent>(TEXT("ViewBoxCenter"));
    ViewBoxCenter->SetupAttachment(TurretGunSprite,TEXT("MuzzleSocket"));
    ViewBoxCenter->SetRelativeLocation(FVector(-140, 0, 0));
    ViewBoxCenter->SetBoxExtent(FVector(48, 32, 56), false);

    ViewSphereCenter = CreateDefaultSubobject<USphereComponent>(TEXT("ViewSphereCenter"));
    ViewSphereCenter->SetupAttachment(TurretGunSprite,TEXT("MuzzleSocket"));
    ViewSphereCenter->SetRelativeLocation(FVector(-74, 0, 0));
    ViewSphereCenter->InitSphereRadius(125.f);

    ViewSphereRight = CreateDefaultSubobject<USphereComponent>(TEXT("ViewSphereRight"));
    ViewSphereRight->SetupAttachment(TurretGunSprite,TEXT("MuzzleSocket"));
    ViewSphereRight->SetRelativeLocation(FVector(-140.f, 0, -50));
    ViewSphereRight->InitSphereRadius(130.f);

    ViewSphereLeft = CreateDefaultSubobject<USphereComponent>(TEXT("ViewSphereLeft"));
    ViewSphereLeft->SetupAttachment(TurretGunSprite,TEXT("MuzzleSocket"));
    ViewSphereLeft->SetRelativeLocation(FVector(-140.f, 0, 50));
    ViewSphereLeft->InitSphereRadius(130.f);*/
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

	TurretGunSprite->AddLocalRotation(FRotator(DeltaTime * Speed * (bReachedRight ? -1 : 1), 0, 0));
	if (TurretGunSprite->GetRelativeRotation().Pitch >= MaxRotationAngle) { bReachedRight = true; }
	else if (TurretGunSprite->GetRelativeRotation().Pitch <= MaxRotationAngle * -1) { bReachedRight = false; }

	if(CurrentTarget != nullptr)
	{
		Shoot();
	}
}

void ATurretBase::Shoot()
{
	GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Purple,"Shoot the dam thing!");
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

void ATurretBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = TurretGunSprite->GetSocketLocation(TEXT("MuzzleSocket"));
	OutRotation = GetShootingRotation();
}

FRotator ATurretBase::GetShootingRotation()const
{
	FRotator res = TurretGunSprite->GetSocketRotation(TEXT("MuzzleSocket"));
	res.Pitch += 180.f;
	return res;
}

