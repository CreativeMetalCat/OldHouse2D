// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Objects/ItemData.h"
#include "PickupInterface.h"
#include "Objects/Hold/HoldableActor.h"
#include "OldHouseCharacter.generated.h"

class UTextRenderComponent;

/**
 * This class is the default character for OldHouse, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class AOldHouseCharacter : public APaperCharacter,public IPickupInterface
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;
protected:

	FTimerHandle StartPossesingTimerHandle;
protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* UnPossesAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	TArray<FItemData> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hold)
	AHoldableActor* CurrentlyHeldActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Death)
	bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Posses)
	float PossesTime = 1.f;
	

	UFUNCTION(BlueprintCallable)
	void HoldObject(AHoldableActor*object);

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


	virtual bool CanBePossesed(){return true;}
	
	void Interact();
	

	void DropItem();

	void PickupItem();

	
	
	void Possess();

	void StartPossess();

	void StopPossess();

	virtual void OnUnPosses();

	virtual void BeginPlay() override;

	virtual bool PickupItem_Implementation(FItemData item) override;

	virtual bool HasItem_Implementation(FItemData item) override;

	virtual void RemoveItem_Implementation(FItemData item) override;

	virtual bool HasKey_Implementation(int keyId) override;

	virtual void RemoveKey_Implementation(int keyId) override;

public:
	AOldHouseCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
