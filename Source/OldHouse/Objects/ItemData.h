// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemData: public FTableRowBase
{
 GENERATED_BODY()
public:
 UPROPERTY(BlueprintReadWrite,EditAnywhere)
 int KeyId = -1;

 UPROPERTY(BlueprintReadWrite,EditAnywhere)
 UPaperSprite* DisplaySprite;

static FItemData ConvertFromPointer(FItemData*data);
};
