// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "BulletBase.generated.h"

UCLASS()
class OLDHOUSE_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	USceneComponent* Scene;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	USphereComponent*Collision;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor*WeaponOwner;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	USoundBase* HitSound;

	UFUNCTION()
    void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
