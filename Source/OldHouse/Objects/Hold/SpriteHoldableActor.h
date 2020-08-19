// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HoldableActor.h"
#include "PaperSpriteComponent.h"

#include "SpriteHoldableActor.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API ASpriteHoldableActor : public AHoldableActor
{
	GENERATED_BODY()


	public:
	ASpriteHoldableActor();

	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UPaperSpriteComponent* Sprite;

	virtual void BeHeld_Implementation(AActor* HoldingActor) override;

	virtual void BeDropped_Implementation(AActor* HoldingActor) override;
};
