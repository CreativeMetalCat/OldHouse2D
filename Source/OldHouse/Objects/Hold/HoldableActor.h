// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OldHouse/Interactions.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HoldableActor.generated.h"

UCLASS()
class OLDHOUSE_API AHoldableActor : public AActor,public IInteractions
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHoldableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* interactor) override;

	virtual bool CanActorBeHeld_Implementation() override;

	
	/*If this actor can not yet be picked up OR if just actor's physics system is needed*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bCanBePickedUp = true;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Break)
	bool bCanBeBroken = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Break)
	bool bBroken = false;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UBoxComponent*ColliderBox;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* Holder = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bHasPhysicsSound = true;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MinVelocityOnHitToProduceSound = 1000.f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MinVelocityOnHitToBreak = 2000.f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Sound)
	USoundBase* BreakSound;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Sound)
	USoundBase* HitSound;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Sound)
	USoundBase* PickupSound;

	//if true actor will not simulate physics until player touches it or pickes up
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Physics)
	bool bFrozenInPlace = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Physics)
	USoundBase* UnFreezeSound;
	

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void BeHeld(AActor* HoldingActor);

	virtual void BeHeld_Implementation(AActor* HoldingActor);

	UFUNCTION(BlueprintCallable)
	virtual void SetFrozenInPlace(bool Frozen);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void BeDropped(AActor* HoldingActor);

	virtual void BeDropped_Implementation(AActor* HoldingActor);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void Break();

	virtual void Break_Implementation();

	UFUNCTION()
    void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
