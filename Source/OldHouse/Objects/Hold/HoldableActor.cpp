// Fill out your copyright notice in the Description page of Project Settings.


#include "HoldableActor.h"


// Sets default values
AHoldableActor::AHoldableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	ColliderBox->SetCollisionResponseToAllChannels(::ECR_Overlap);
}

// Called when the game starts or when spawned
void AHoldableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHoldableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHoldableActor::Interact_Implementation(AActor* interactor)
{
	
}

bool AHoldableActor::CanActorBeHeld_Implementation()
{
	return (bCanBePickedUp && Holder == nullptr);
}

void AHoldableActor::BeHeld_Implementation(AActor* HoldingActor)
{
	SetActorEnableCollision(false);
	Holder = HoldingActor;
	if(PickupSound!=nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickupSound,GetActorLocation(),GetActorRotation());
	}
}

void AHoldableActor::BeDropped_Implementation(AActor* HoldingActor)
{
	SetActorEnableCollision(true);
	Holder = nullptr;
	if(PickupSound!=nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickupSound,GetActorLocation(),GetActorRotation());
	}
}

void AHoldableActor::Break_Implementation()
{
	if(BreakSound!=nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),BreakSound,GetActorLocation(),GetActorRotation());
	}
}

