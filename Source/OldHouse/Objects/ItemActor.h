// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Components/BoxComponent.h"
#include "OldHouse/Interactions.h"
#include "ItemActor.generated.h"

/*
 *DO NOT USE THIS CLASS DIRECTLY.
 *Basic actor that stores item data and can be interacted with
*/
UCLASS()
class OLDHOUSE_API AItemActor : public AActor,public IInteractions
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UPaperSpriteComponent*Sprite;
	
	UPROPERTY(Category =Collision, VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionBox;
	
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FItemData Item;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FDataTableRowHandle ItemData;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Interact_Implementation(AActor* interactor) override;
};
