// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AButtonBase::AButtonBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    RootComponent = Sprite;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment((RootComponent));
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	
}

// Called when the game starts or when spawned
void AButtonBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AButtonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AButtonBase::Interact_Implementation(AActor* interactor)
{
	if(!bIsPressed)
	{
		OnButtonPressed();
		OnButtonClicked.Broadcast();
		if(bIsTimed)
		{
			GetWorldTimerManager().SetTimer(ButtonUnclickTimerHandle,this,&AButtonBase::OnButtonUnPressed,UnClickTime);
		}
		bIsPressed = true;
	}
	else if(bCanBeUnPressed)
	{
		OnButtonUnPressed();
		OnButtonUnClicked.Broadcast();
		bIsPressed = false;
	}
}

void AButtonBase::OnButtonPressed()
{
	if(UseSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),UseSound,GetActorLocation(),GetActorRotation());
	}
	if(Sprite != nullptr)
	{
		if(ClickedStateSprite != nullptr)
		{
			Sprite->SetSprite(ClickedStateSprite);
		}
	}
}

void AButtonBase::OnButtonUnPressed()
{
	if(UseSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),UseSound,GetActorLocation(),GetActorRotation());
	}
	if(Sprite != nullptr)
	{
		if(UnClickedStateSprite != nullptr)
		{
			Sprite->SetSprite(UnClickedStateSprite);
		}
	}
	bIsPressed = false;
	OnButtonUnClicked.Broadcast();
}

