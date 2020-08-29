// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OldHouse/OldHouseCharacter.h"
#include "PatrollingMovement/PatrolSystemBase.h"

#include "AIHuman.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API AAIHuman : public AOldHouseCharacter
{
	GENERATED_BODY()
	public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category= AIWeapon)
	TSubclassOf<AWeaponBase> DefaultWeaponClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category= Patrolling)
	APatrolSystemBase * CurrentPatrollingSystem;
	
	virtual void OnUnPosses() override;

	virtual bool CanBePossesed() override;

	virtual void OnPosses(AOldHouseCharacter*originalBody) override;

	virtual void BeginPlay() override;
};
