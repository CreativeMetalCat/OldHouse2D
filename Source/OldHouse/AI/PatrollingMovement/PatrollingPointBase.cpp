// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrollingPointBase.h"

// Sets default values
APatrollingPointBase::APatrollingPointBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorBillboard"));
	RootComponent = EditorBillboard;
}

// Called when the game starts or when spawned
void APatrollingPointBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrollingPointBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

