// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"
#include "OldHouse/OldHouseCharacter.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABulletBase::ABulletBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collsion"));
	Collision->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::OnCollisionBeginOverlap);

	
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletBase::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != GetOwner())
	{
		if(HitSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),HitSound,GetActorLocation(),GetActorRotation());
		}
		AOldHouseCharacter*human = Cast<AOldHouseCharacter>(OtherActor);
		if(human != nullptr)
		{
			if(!human->bHiddenInShadow)//you can not kill this human because it is out of line of fire
			{
				if (human->Tags.Find("Player") != -1)
				{
					if (Tags.Find("ShotByPlayer") != -1)
					{
						//this bullet is shot by player so we ignore it 
					}
					else if (Tags.Find("ShotByEnemy") != -1)
					{
						human->Die();
						Destroy();
					}				
				}
				else if(human->Tags.Find("Enemy") != -1)
				{
					if (Tags.Find("ShotByPlayer") != -1)
					{				
						human->Die();
						Destroy();
					}
					else if (Tags.Find("ShotByEnemy") != -1)
					{
					
					}	
				}
			}
		}
	}
}

