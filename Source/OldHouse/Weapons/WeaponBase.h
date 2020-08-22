// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponAnimTypes.h"
#include "WeaponBase.generated.h"

UCLASS()
class OLDHOUSE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float Damage = 10.f;

	//this might be ignored by character. It's more of suggestion rather than demand
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EWeaponAnimType AnimType = EWeaponAnimType::EWT_MeleeKnife;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor*WeaponOwner;
};
