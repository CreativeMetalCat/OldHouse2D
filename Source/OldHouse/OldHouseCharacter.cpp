// Copyright Epic Games, Inc. All Rights Reserved.

#include "OldHouseCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Interactions.h"
#include "Camera/CameraComponent.h"
#include "Player/PossesivePlayerController.h"
#include "Engine.h"
#include "Weapons/KnifeBase.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AOldHouseCharacter

AOldHouseCharacter::AOldHouseCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	WallGrabBox=CreateDefaultSubobject<UBoxComponent>(TEXT("WallGrabBox"));
	WallGrabBox->SetupAttachment(RootComponent);
	WallGrabBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WallGrabBox->SetBoxExtent(FVector(24,32,8));
	WallGrabBox->SetRelativeLocation(FVector(50,0,20));

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D Splatformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);

	WallGrabBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WallGrabBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Overlap);

	WallGrabBox->OnComponentBeginOverlap.AddDynamic(this, &AOldHouseCharacter::OnWallGrabBoxBeginOverlap);
}

//////////////////////////////////////////////////////////////////////////
// Animation

void AOldHouseCharacter::HoldObject(AHoldableActor* object)
{
	if(object != nullptr)
	{
		CurrentlyHeldActor = object;
		object->BeHeld(this);
		
		object->AttachToComponent(GetSprite(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("ObjectHolding"));
	}	
}

bool AOldHouseCharacter::SetWeapon(TSubclassOf<AWeaponBase> WeaponClass)
{
	Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
	if(Weapon != nullptr)
	{
		Weapon->AttachToComponent(GetSprite(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetWeaponAttachmentSocketName(Weapon->AnimType));
		Weapon->WeaponOwner = this;
		return true;
	}
	return false;
}

void AOldHouseCharacter::UpdateAnimation()
{
	if(!bDead && !bPlayingMeleeAttackAnim)
	{
		// Are we moving or standing still?
		UPaperFlipbook* DesiredAnimation = GetDesiredAnimation(); 
		if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
		{
			GetSprite()->SetFlipbook(DesiredAnimation);
		}
		if (!GetSprite()->IsLooping()) { GetSprite()->SetLooping(true); GetSprite()->PlayFromStart(); }
	}
}

void AOldHouseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
}


//////////////////////////////////////////////////////////////////////////
// Input

void AOldHouseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOldHouseCharacter::MoveRight);

	PlayerInputComponent->BindAction("Interact",IE_Pressed,this,&AOldHouseCharacter::Interact);

	PlayerInputComponent->BindAction("Possess",IE_Pressed,this,&AOldHouseCharacter::StartPossess);
	PlayerInputComponent->BindAction("Possess",IE_Released,this,&AOldHouseCharacter::StopPossess);

	PlayerInputComponent->BindAction("DropItem",IE_Pressed,this,&AOldHouseCharacter::PickupItem);
	
	PlayerInputComponent->BindAction("Attack",IE_Pressed,this,&AOldHouseCharacter::Attack);
}

bool AOldHouseCharacter::CanBeSeen()
{
	return !(bDead || bHiddenInShadow);
}

void AOldHouseCharacter::Interact()
{
	TArray<AActor*> actors;
	GetCapsuleComponent()->GetOverlappingActors(actors);
	if (actors.Num() > 0)
	{
		for (int i = 0; i < actors.Num(); i++)
		{
			if (actors[i]->Implements<UInteractions>() || (Cast<IInteractions>(actors[i]) != nullptr))
			{
				IInteractions::Execute_Interact(actors[i], this);
			}
		}
	}
}

void AOldHouseCharacter::Die()
{
	if (!bDead)
	{
		APossesivePlayerController*PC = Cast<APossesivePlayerController>(GetController());
		if ( PC != nullptr)
		{
			DisableInput(PC);
		}
		bDead = true;
		if (DeathAnimation != nullptr)
		{
			GetSprite()->SetFlipbook(DeathAnimation);
			GetSprite()->SetLooping(false);
		}
		if (DeathSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), GetActorRotation());
		}
		if (GetController() != nullptr)
		{
			if(Cast<APlayerController>(GetController()) == nullptr)
			{
				GetController()->UnPossess();
			}
		}
		if(Weapon != nullptr)
		{
			Weapon->Destroy();
			Weapon = nullptr;
		}
	}
}

void AOldHouseCharacter::DropItem()
{
	if (CurrentlyHeldActor != nullptr)
	{	
		CurrentlyHeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentlyHeldActor->BeDropped(this);
		CurrentlyHeldActor->SetActorLocation(GetSprite()->GetSocketLocation(TEXT("DropItemPoint")));
		CurrentlyHeldActor = nullptr;
	}
}

void AOldHouseCharacter::PickupItem()
{
	if (CurrentlyHeldActor == nullptr)
	{
		TArray<AActor*> actors;
		GetCapsuleComponent()->GetOverlappingActors(actors);
		if (actors.Num() > 0)
		{
			for (int i = 0; i < actors.Num(); i++)
			{
				if (actors[i]->Implements<UInteractions>() || (Cast<IInteractions>(actors[i]) != nullptr))
				{
					if (IInteractions::Execute_CanActorBeHeld((actors[i])) && CurrentlyHeldActor == nullptr)
					{
						HoldObject(Cast<AHoldableActor>(actors[i]));
					}
				}
			}
		}
	}
	else
	{
		DropItem();
	}
	
}

UPaperFlipbook* AOldHouseCharacter::GetDesiredAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
	
	if(Weapon != nullptr)
	{
		switch (Weapon->AnimType)
		{
		case EWeaponAnimType::EWT_Pistol:
			return (PlayerSpeedSqr > 0.0f) ? PistolWalkAnimation : PistolIdleAnimation;
			break;
			
		case EWeaponAnimType::EWT_MeleeKnife:
			return (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
			break;
			
		default:
			return (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
			break;
		}
	}
	else
	{
		
		return (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	}
}

FVector AOldHouseCharacter::GetWeaponSocketLocation() const
{
	if(Weapon != nullptr)
	{
		return GetSprite()->GetSocketLocation(GetWeaponAttachmentSocketName(Weapon->AnimType));
	}
	else
	{
		return GetSprite()->GetSocketLocation(TEXT("WeaponHolding"));
	}
}

FRotator AOldHouseCharacter::GetWeaponSocketRotation() const
{
	if(Weapon != nullptr)
	{
		return GetSprite()->GetSocketRotation(GetWeaponAttachmentSocketName(Weapon->AnimType));
	}
	else
	{
		return GetSprite()->GetSocketRotation(TEXT("WeaponHolding"));
	}
}

FName AOldHouseCharacter::GetWeaponAttachmentSocketName(EWeaponAnimType animType)const
{
	switch (animType)
	{
	case EWeaponAnimType::EWT_Pistol:
		return TEXT("PistolHolding");
		break;
	case EWeaponAnimType::EWT_MeleeKnife:
		return TEXT("WeaponHolding");
		break;
	default:
		return TEXT("WeaponHolding");
		break;
	}
}



void AOldHouseCharacter::Attack()
{
	if(Weapon != nullptr)
	{
		if(Cast<AKnifeBase>(Weapon) != nullptr)
		{
			if(!EndMeleeAttackAnimTimerHandle.IsValid())
			{
				if(StabAnimation != nullptr)
				{
					GetSprite()->SetLooping(false);
					bPlayingMeleeAttackAnim = true;
					GetSprite()->SetFlipbook(StabAnimation);
					GetSprite()->PlayFromStart();
					GetWorldTimerManager().SetTimer(EndMeleeAttackAnimTimerHandle,this,&AOldHouseCharacter::EndMeleeAttackAnim,GetSprite()->GetFlipbookLength());
				}
				else
				{
					Cast<AKnifeBase>(Weapon)->DealDamage();
				}
			}
		}
		else
		{
			Weapon->Fire(GetWeaponSocketLocation(),GetWeaponSocketRotation());
		}
	}
}

void AOldHouseCharacter::EndMeleeAttackAnim()
{
	GetWorldTimerManager().ClearTimer(EndMeleeAttackAnimTimerHandle);
	if(StabAnimation != nullptr)
	{
		Cast<AKnifeBase>(Weapon)->DealDamage();
		GetSprite()->ReverseFromEnd();
		GetWorldTimerManager().SetTimer(FinishAttackAnimTimerHandle,this,&AOldHouseCharacter::FinishMeleeAttack,GetSprite()->GetFlipbookLength());
	}
}

void AOldHouseCharacter::FinishMeleeAttack()
{
	bPlayingMeleeAttackAnim = false;
}

void AOldHouseCharacter::Possess()
{
	GetWorldTimerManager().ClearTimer(StartPossesingTimerHandle);
	if (GetController() != nullptr)
	{
		APossesivePlayerController*PC = Cast<APossesivePlayerController>(GetController());
		if ( PC != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Emerald,"Shooting line");
			FHitResult hit;
			PC->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery2,false,hit);
			if(hit.bBlockingHit)
			{
				if(hit.Actor != nullptr)
				{
					AOldHouseCharacter* Other = Cast<AOldHouseCharacter>(hit.GetActor());
					if(Other!=nullptr)
					{
						if(Other->CanBePossesed())
						{
							OnUnPosses();
							Other->OnPosses();
							PC->OnChangedBodies();
							PC->Possess(Other);
						}
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Yellow,"Actor is not possesable. Actor name: " + hit.GetActor()->GetName());
						
					}
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Emerald,"Hit but found no actor");
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"Didn't hit");
				GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,hit.TraceStart.ToString());
			}
		}
	}
}

void AOldHouseCharacter::StartPossess()
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Emerald,"Starting...");
	if(!StartPossesingTimerHandle.IsValid())
	{
		GetWorldTimerManager().SetTimer(StartPossesingTimerHandle,this,&AOldHouseCharacter::Possess,PossesTime);
	}
}

void AOldHouseCharacter::StopPossess()
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Emerald,"Aborting...");
	GetWorldTimerManager().ClearTimer(StartPossesingTimerHandle);
}

void AOldHouseCharacter::OnUnPosses()
{
	bControlledByPlayer = false;
}

void AOldHouseCharacter::OnPosses()
{
	bControlledByPlayer = true;
}

void AOldHouseCharacter::BeginPlay()
{
	Super::BeginPlay();

	WallGrabBox->OnComponentBeginOverlap.AddDynamic(this, &AOldHouseCharacter::OnWallGrabBoxBeginOverlap);

	WallGrabBox->OnComponentEndOverlap.AddDynamic(this, &AOldHouseCharacter::OnWallGrabBoxEndOverlap);
}

float AOldHouseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(!bDead){Die();}
	return DamageAmount;
}

bool AOldHouseCharacter::PickupItem_Implementation(FItemData item)
{
	Items.Add(item);
	return true;
}

bool AOldHouseCharacter::HasItem_Implementation(FItemData item)
{
	if (Items.Num() > 0)
	{
		for (int i = 0; i < Items.Num(); i++)
		{
			if (Items[i].EngineName == item.EngineName)
			{
				return true;
			}
		}
	}
	
	return false;
}

void AOldHouseCharacter::RemoveItem_Implementation(FItemData item)
{
	if (Items.Num() > 0)
	{
		for (int i = 0; i < Items.Num(); i++)
		{
			if (Items[i].EngineName == item.EngineName)
			{
				Items.RemoveAt(i);
			}
		}
	}
}

bool AOldHouseCharacter::HasKey_Implementation(int keyId)
{
	if (Items.Num() > 0)
	{
		for (int i = 0; i < Items.Num(); i++)
		{
			if (Items[i].KeyId == keyId && Items[i].bIsKey)
			{
				return true;
			}
		}
	}
	
	return false;
}

void AOldHouseCharacter::RemoveKey_Implementation(int keyId)
{
	if (Items.Num() > 0)
	{
		for (int i = 0; i < Items.Num(); i++)
		{
			if (Items[i].KeyId == keyId && Items[i].bIsKey)
			{
				Items.RemoveAt(i);
				return;
			}
		}
	}
}

bool AOldHouseCharacter::CanJumpInternal_Implementation() const
{
	if(!bIsHoldingWall)
	{
		
		// Ensure the character isn't currently crouched.
		bool bCanJump = !bIsCrouched;

		// Ensure that the CharacterMovement state is valid
		bCanJump &= GetCharacterMovement()->CanAttemptJump();

		if (bCanJump)
		{
			// Ensure JumpHoldTime and JumpCount are valid.
			if (!bWasJumping || GetJumpMaxHoldTime() <= 0.0f)
			{
				if (JumpCurrentCount == 0 && GetCharacterMovement()->IsFalling())
				{
					bCanJump = JumpCurrentCount + 1 < JumpMaxCount;
				}
				else
				{
					bCanJump = JumpCurrentCount < JumpMaxCount;
				}
			}
			else
			{
				// Only consider JumpKeyHoldTime as long as:
				// A) The jump limit hasn't been met OR
				// B) The jump limit has been met AND we were already jumping
				const bool bJumpKeyHeld = (bPressedJump && JumpKeyHoldTime < GetJumpMaxHoldTime());
				bCanJump = bJumpKeyHeld &&
                            ((JumpCurrentCount < JumpMaxCount) || (bWasJumping && JumpCurrentCount == JumpMaxCount));
			}
		}

		return bCanJump;
	}
	else
	{
		return true;
	}
	
}


void AOldHouseCharacter::OnWallGrabBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (GetCharacterMovement() != nullptr)
	{
		if(!GetCharacterMovement()->IsMovingOnGround())
		{
			GetCharacterMovement()->GravityScale = 0.f;
			GetCharacterMovement()->Velocity = FVector(0,0,0);
			GetCharacterMovement()->StopActiveMovement();
			bIsHoldingWall = true;
		}
	}
}

void AOldHouseCharacter::OnWallGrabBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetCharacterMovement() != nullptr)
	{
		GetCharacterMovement()->GravityScale = 2.f;
		bIsHoldingWall = false;
	}
}

void AOldHouseCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	if(!bPlayingMeleeAttackAnim)
	{
		// Apply the input to the character motion
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
}

void AOldHouseCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void AOldHouseCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void AOldHouseCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	if(!bIsHoldingWall || !bControlledByPlayer)
	{
		// Set the rotation so that the character faces his direction of travel.
		if (Controller != nullptr)
		{
			if (TravelDirection < 0.0f)
			{
				Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
				if(Weapon != nullptr)
				{
					if(Weapon->GetActorLocation().Y!=GetWeaponSocketLocation().Y+0.02)
					{
						Weapon->SetActorLocation(FVector(GetWeaponSocketLocation().X,GetWeaponSocketLocation().Y + 0.02, GetWeaponSocketLocation().Z));						
					}
				}
			}
			else if (TravelDirection > 0.0f)
			{
				Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
				if(Weapon != nullptr)
				{
					if(Weapon->GetActorLocation().Y!=GetSprite()->GetSocketLocation(GetWeaponAttachmentSocketName(Weapon->AnimType)).Y)
					{
						Weapon->SetActorLocation(GetWeaponSocketLocation());						
					}
				}
			}
		}
	}
}
