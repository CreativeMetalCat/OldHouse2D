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

 //MUST BE UNIQUE OR SOME FUNCTIONS WILL NOT GIVE PROPER RESULT
 UPROPERTY(BlueprintReadWrite,EditAnywhere)
 FString EngineName;
 
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Key)
 int KeyId = -1;

 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Key)
 bool bIsKey=false;
 
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Display)
 UPaperSprite* DisplaySprite;

static FItemData ConvertFromPointer(FItemData*data);

};
