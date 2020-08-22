// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorBase.h"
#include "Kismet/GameplayStatics.h"
#include "OldHouse/PickupInterface.h"

#include "PaperFlipbookComponent.h"

// Sets default values
ADoorBase::ADoorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment((RootComponent));
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void ADoorBase::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorBase::Toggle_Implementation()
{
	if (bOpen)
	{
		if (CloseSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), CloseSound, GetActorLocation(), GetActorRotation());
		}
		bOpen = false;
	}
	else
	{
		if (OpenSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation(), GetActorRotation());
		}
		bOpen = true;
	}
}

// Called every frame
void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorBase::Interact_Implementation(AActor* interactor)
{
	if(CanBeToggled())
	{
		if (!bLocked)
		{
			Toggle();
		}
		else
		{
			if (interactor->Implements<UPickupInterface>() || (Cast<IPickupInterface>(interactor) != nullptr))
			{
				if(IPickupInterface::Execute_HasKey(interactor, KeyId))
				{
					bLocked = false;
					IPickupInterface::Execute_RemoveKey(interactor,KeyId);
					if (UnlockSound != nullptr)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), UnlockSound, GetActorLocation(), GetActorRotation());
					}
				}
			}
		}
	}
}

