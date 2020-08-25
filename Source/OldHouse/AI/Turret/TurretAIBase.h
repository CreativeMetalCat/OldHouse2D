// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "OldHouse/AI/Turret/TurretBase.h"
#include "TurretAIBase.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API ATurretAIBase : public AAIController
{
	GENERATED_BODY()
protected:
	FTimerHandle UpdateTimerHandle;
public:
	ATurretAIBase();

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UAIPerceptionComponent*Senses;

	//turret controller by this controller
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	ATurretBase* Turret;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	bool bCanSeeTarget = false;

	virtual void BeginPlay() override;

	virtual void UpdatePerceivedActors();

	virtual void Tick(float DeltaSeconds) override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
