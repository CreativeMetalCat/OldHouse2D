// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OldHouse/Interactions.h"
#include "Components/BoxComponent.h"
#include "HiddingPlaceBase.generated.h"

UCLASS()
class OLDHOUSE_API AHiddingPlaceBase : public AActor,public IInteractions
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHiddingPlaceBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UBoxComponent*ColliderBox;

	virtual void Interact_Implementation(AActor* interactor) override;
};
