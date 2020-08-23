// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "HumanAIBase.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API AHumanAIBase : public AAIController
{
	GENERATED_BODY()
	protected:
	FTimerHandle UpdateTimerHandle;
	public:
	AHumanAIBase();
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UAIPerceptionComponent*Senses;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bCanSeeEnemy = false;
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	virtual void UpdatedSeenActors(const TArray<AActor*>& actors);

	virtual void BeginPlay() override;

	virtual void UpdatePerceivedActors();
};
