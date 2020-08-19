// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HoldableActor.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "AnimatedHoldableActor.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API AAnimatedHoldableActor : public AHoldableActor
{
	GENERATED_BODY()
	public:
	AAnimatedHoldableActor();
	
	UPROPERTY(Category = Animation, EditDefaultsOnly, BlueprintReadWrite)
	UPaperFlipbookComponent* Sprite;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UBoxComponent* PhysicsBox;

	virtual void BeHeld_Implementation(AActor* HoldingActor) override;

	virtual void BeDropped_Implementation(AActor* HoldingActor) override;
};
