// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "UObject/ObjectMacros.h"

#include "OldHouseGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OLDHOUSE_API UOldHouseGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()
	public:
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Checkpoint)
	FVector LastCheckpointLocation;

	//set this to false each time new map is open
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Checkpoint)
	bool bHasHitCheckpoint = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Levels)
	TArray<FString> LoadedLevels;
};
