// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "OldHouse/PickupInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
//#include "IDetailTreeNode.h"

// Sets default values
AItemActor::AItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Spite"));
	RootComponent = Sprite;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment((RootComponent));
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);


}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnBoxBeginOverlap);
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (bPickupOnOverlap)
	{
		BePickedUp(OtherActor);
	}
}

void AItemActor::OnConstruction(const FTransform& Transform)
{
	
	//this is placeholder implementation. Due to the way Unreal is created it's easier and safer to implement this system in BP
	if(ItemData.DataTable!=nullptr)
	{
		FItemData* data = ItemData.GetRow<FItemData>(ItemData.RowName.ToString());
		if (data != nullptr)
		{
			Item = FItemData::ConvertFromPointer(data);
		}
	}

	if(Item.DisplaySprite != nullptr)
	{
		Sprite->SetSprite(Item.DisplaySprite);
	}
}

void AItemActor::Interact_Implementation(AActor* interactor)
{
	if(!bPickupOnOverlap)
	{
		BePickedUp(interactor);
	}
}

void AItemActor::BePickedUp(AActor* interactor)
{
	if (interactor->Implements<UPickupInterface>() || (Cast<IPickupInterface>(interactor) != nullptr))
	{
		if(IPickupInterface::Execute_PickupItem(interactor, Item))
		{
			if(PickupSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickupSound,GetActorLocation(),GetActorRotation());
			}
			Destroy();
		}
	}
}
