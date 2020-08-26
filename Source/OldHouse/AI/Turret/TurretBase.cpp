// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretBase.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATurretBase::ATurretBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    TurretBaseSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TurretBaseSprite"));
    RootComponent = TurretBaseSprite;

    TurretGunSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("TurretGunSprite"));
    TurretGunSprite->SetupAttachment(RootComponent,TEXT("GunSocket"));

	ViewConeDisplaySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ViewConeDisplaySprite"));
	ViewConeDisplaySprite->SetupAttachment(TurretGunSprite,TEXT("MuzzleSocket"));
	ViewConeDisplaySprite->SetRelativeScale3D_Direct(FVector(3.75f,1,2.75f));
	ViewConeDisplaySprite->SetRelativeLocation(FVector(-110.f,0,0));
	ViewConeDisplaySprite->SetRelativeRotation(FRotator(180,0,-180));

	ViewConeLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("ViewConeLight"));
	ViewConeLight->SetupAttachment(TurretGunSprite,TEXT("MuzzleSocket"));
	ViewConeLight->SetRelativeRotation(FRotator(180,0,-180));

	AlarmAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("AlarmSound"));
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

	

	if(CurrentTarget != nullptr)
	{
		if(!AlarmAudio->IsPlaying())
		{
			AlarmAudio->Play();
		}
		TurretGunSprite->SetRelativeRotation(GetAimRotation());

		//temp -- start
		//TurretGunSprite->AddLocalRotation(FRotator(DeltaTime * Speed * (bReachedRight ? -1 : 1), 0, 0));
		//if (TurretGunSprite->GetRelativeRotation().Pitch >= MaxRotationAngle) { bReachedRight = true; }
		//else if (TurretGunSprite->GetRelativeRotation().Pitch <= MaxRotationAngle * -1) { bReachedRight = false; }
		//temp -- end
		
		if(ViewConeDisplaySprite != nullptr)
		{
			if(ViewConeDisplaySprite->GetSpriteColor() != AlertColor)
			{
				ViewConeDisplaySprite->SetSpriteColor(AlertColor);
			}
		}
		Shoot();
	}
	else
	{
		TurretGunSprite->AddLocalRotation(FRotator(DeltaTime * Speed * (bReachedRight ? -1 : 1), 0, 0));
		if (TurretGunSprite->GetRelativeRotation().Pitch >= MaxRotationAngle) { bReachedRight = true; }
		else if (TurretGunSprite->GetRelativeRotation().Pitch <= MaxRotationAngle * -1) { bReachedRight = false; }

		if(AlarmAudio->IsPlaying())
		{
			AlarmAudio->StopDelayed(0.5f);
		}
		
		if(ViewConeDisplaySprite != nullptr)
		{
			if(ViewConeDisplaySprite->GetSpriteColor() != NormalColor)
			{
				ViewConeDisplaySprite->SetSpriteColor(NormalColor);
			}
		}
	}
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

void ATurretBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = TurretGunSprite->GetSocketLocation(TEXT("MuzzleSocket"));
	FRotator Rot = TurretGunSprite->GetSocketRotation(TEXT("MuzzleSocket"));
	Rot.Yaw -= TurretGunSprite->GetSocketTransform(TEXT("MuzzleSocket"), RTS_ParentBoneSpace).Rotator().Roll;
	Rot.Pitch += 180.f;
	Rot.Roll += 180.f;
	OutRotation = Rot;
}

FRotator ATurretBase::GetAimRotation()const
{	
	if (CurrentTarget != nullptr)
	{
		FRotator res;
		FVector ThisLocation = FVector(GetActorLocation().X,0,GetActorLocation().Z);
		FVector TargetLocation = FVector(CurrentTarget->GetActorLocation().X,0,CurrentTarget->GetActorLocation().Z);
		res = UKismetMathLibrary::FindLookAtRotation(ThisLocation, TargetLocation);
		res = res.Clamp();
		
		if(res.Yaw >= 180)
		{
			res.Pitch -= 90.f;
		}
		else
		{
			res.Pitch += 90.f;
		}
		return res;
	}
	else
	{
		return TurretGunSprite->GetRelativeRotation();
	}		
}

FRotator ATurretBase::GetShootingRotation() const
{
	FRotator res;
	
	res = TurretGunSprite->GetSocketRotation(TEXT("MuzzleSocket"));
	res.Roll = 0;
	if(res.Yaw >= 180)
	{
		res.Pitch -= 180.f;
	}
	else
	{
		res.Pitch += 180.f;
	}
	return res;
}

