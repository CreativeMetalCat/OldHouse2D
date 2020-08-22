// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "OldHouse/Objects/DoorBase.h"
#include "Paper2D/Classes/PaperFlipbook.h"
#include "AnimatedDoor.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API AAnimatedDoor : public ADoorBase
{
	GENERATED_BODY()
	
	protected:
	FTimerHandle StopMoveAnimTimerHandle;

	bool bIsMoving = false;
	
	public:
	AAnimatedDoor();

	UFUNCTION(BlueprintPure)
	bool GetIsMoving() const { return bIsMoving; }
	
	UPROPERTY(Category =Sprite, VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* Sprite;

	UPROPERTY(Category =Animations, EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* ClosedAnim;

	UPROPERTY(Category =Animations, EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* MoveAnim;

	virtual void OnToggle() override;

	virtual void OnMoveAnimStopped();

	virtual bool CanBeToggled() override;
};
