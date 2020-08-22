// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OldHouse/Objects/Button/ButtonBase.h"
#include "OldHouse/Objects/DoorBase.h"
#include "DoorButton.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API ADoorButton : public AButtonBase
{
	GENERATED_BODY()
protected:
	void ToggleDoors();
public:
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<ADoorBase*> Doors;

	virtual void OnButtonPressed() override;

	virtual void OnButtonUnPressed() override;
};


