// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "WeaponAnimTypes.generated.h"

UENUM(BlueprintType)
enum class EWeaponAnimType: uint8
{
    EWT_MeleeKnife UMETA(DisplayName = "MeleeKnife"),
    EWT_Pistol UMETA(DisplayName = "Pistol")
};