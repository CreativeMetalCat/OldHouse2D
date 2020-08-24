// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddingPlaceBase.h"
#include "OldHouse/OldHouseCharacter.h"

// Sets default values
AHiddingPlaceBase::AHiddingPlaceBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ColliderBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	ColliderBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ColliderBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AHiddingPlaceBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHiddingPlaceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHiddingPlaceBase::Interact_Implementation(AActor* interactor)
{
	AOldHouseCharacter*human = Cast<AOldHouseCharacter>(interactor);
	if(human != nullptr)
	{
		human->SetHiddenInTheShadow(!human->bHiddenInShadow);
	}
}

