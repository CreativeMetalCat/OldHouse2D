// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OldHouse/Weapons/WeaponBase.h"
#include "PaperSpriteComponent.h"
#include "Projectiles/BulletBase.h"
#include "PistolBase.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API APistolBase : public AWeaponBase
{
	GENERATED_BODY()
	public:

	APistolBase();

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UPaperSpriteComponent* Sprite;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<ABulletBase> BulletClass;

	virtual bool Fire(FVector Location,FRotator Rotaion) override;
};
