// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OldHouse//Objects/DoorBase.h"
#include "SpriteDoor.generated.h"

/**
 * 
 */
UCLASS()
class VICTOR_API ASpriteDoor : public ADoorBase
{
	GENERATED_BODY()
	public:
	ASpriteDoor();
	
	UPROPERTY(Category =Sprite, VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* SpriteOpenedState;

	UPROPERTY(Category =Sprite, VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* SpriteClosedState;

	virtual void Toggle_Implementation() override;
};
