// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OldHouse/Interactions.h"
#include "Components/BoxComponent.h"
#include "HoldableActor.generated.h"

UCLASS()
class OLDHOUSE_API AHoldableActor : public AActor,public IInteractions
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHoldableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* interactor) override;

	virtual bool CanActorBeHeld_Implementation() override;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UBoxComponent*ColliderBox;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* Holder = nullptr;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void BeHeld(AActor* HoldingActor);

	virtual void BeHeld_Implementation(AActor* HoldingActor);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void BeDropped(AActor* HoldingActor);

	virtual void BeDropped_Implementation(AActor* HoldingActor);
};
