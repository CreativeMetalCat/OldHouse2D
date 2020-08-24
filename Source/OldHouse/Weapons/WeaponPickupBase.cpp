// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickupBase.h"
#include "OldHouse/OldHouseCharacter.h"

// Sets default values
AWeaponPickupBase::AWeaponPickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ColliderBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	ColliderBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ColliderBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void AWeaponPickupBase::BeginPlay()
{
	Super::BeginPlay();
	
	ColliderBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPickupBase::OnBoxBeginOverlap);
}

// Called every frame
void AWeaponPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponPickupBase::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOldHouseCharacter*human = Cast<AOldHouseCharacter>(OtherActor);
	if(human != nullptr)
	{
		if(human->Weapon == nullptr)
		{
			human->SetWeapon(WeaponClass);
			Destroy();
		}
	}
}

