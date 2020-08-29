// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolSystemBase.h"

// Sets default values
APatrolSystemBase::APatrolSystemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorBillboard"));
	RootComponent = EditorBillboard;
}

// Called when the game starts or when spawned
void APatrolSystemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolSystemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

