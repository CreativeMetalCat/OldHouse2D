// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "OldHouse/Weapons/WeaponBase.h"
#include "KnifeBase.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API AKnifeBase : public AWeaponBase
{
	GENERATED_BODY()
	
	public:
	AKnifeBase();
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UPaperSpriteComponent* Sprite;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UBoxComponent* DamageBox;

	virtual void DealDamage();

	virtual void SetHiddenInShadow(bool Hidden) override;
};
