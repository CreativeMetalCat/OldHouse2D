// Fill out your copyright notice in the Description page of Project Settings.


#include "HoldableActor.h"


// Sets default values
AHoldableActor::AHoldableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	ColliderBox->SetCollisionResponseToAllChannels(::ECR_Overlap);

	ColliderBox->OnComponentBeginOverlap.AddDynamic(this, &AHoldableActor::OnBoxBeginOverlap);
}

// Called when the game starts or when spawned
void AHoldableActor::BeginPlay()
{
	Super::BeginPlay();
	ColliderBox->OnComponentBeginOverlap.AddDynamic(this, &AHoldableActor::OnBoxBeginOverlap);
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

void AHoldableActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasPhysicsSound)
	{
		if (OtherActor->GetVelocity().Size() > MinVelocityOnHitToProduceSound || GetVelocity().Size() > MinVelocityOnHitToProduceSound)
		{
			if (HitSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation(), GetActorRotation());
			}
		}

		if ((OtherActor->GetVelocity().Size() > MinVelocityOnHitToBreak || GetVelocity().Size() > MinVelocityOnHitToBreak)&&!bBroken)
		{
			bBroken = true;
			Break();
			if (BreakSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSound, GetActorLocation(), GetActorRotation());
			}
		}
	}
}

