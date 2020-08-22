// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"
#include "OldHouse/Interactions.h"
#include "Components/BoxComponent.h"
#include "ButtonBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonUnClicked);

UCLASS()
class OLDHOUSE_API AButtonBase : public AActor, public IInteractions
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AButtonBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle ButtonUnclickTimerHandle;
public:
	UPROPERTY(BlueprintAssignable)
	FOnButtonClicked OnButtonClicked;
	
	UPROPERTY(BlueprintAssignable)
	FOnButtonClicked OnButtonUnClicked;
		
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	USoundBase* UseSound;

	UPROPERTY(Category =Sprite, VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* Sprite;

	UPROPERTY(Category =Collision, VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionBox;

	UPROPERTY(Category =Sprite,EditAnywhere, BlueprintReadWrite)
	UPaperSprite* ClickedStateSprite;

	UPROPERTY(Category =Sprite,EditAnywhere, BlueprintReadWrite)
	UPaperSprite* UnClickedStateSprite;

	UPROPERTY(Category =State,EditAnywhere, BlueprintReadWrite)
	bool bIsPressed = false;

	//if true, using button when it's in Pressed state will return it into the unPressed state
	UPROPERTY(Category =State,EditAnywhere, BlueprintReadWrite)
	bool bCanBeUnPressed = false;

	//if true button will be unPressed after some time
	UPROPERTY(Category =State,EditAnywhere, BlueprintReadWrite)
	bool bIsTimed = false;
	
	UPROPERTY(Category =State,EditAnywhere, BlueprintReadWrite)
	float UnClickTime = 5.f;

	virtual void Interact_Implementation(AActor* interactor) override;

	virtual void OnButtonPressed();

	virtual void OnButtonUnPressed();
};
