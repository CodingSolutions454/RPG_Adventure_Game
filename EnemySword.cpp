// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySword.h"
#include "AI_Bot_Character.h"

// Sets default values
AEnemySword::AEnemySword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates a body for this EnemySword.
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AEnemySword::BeginPlay()
{
	Super::BeginPlay();

	// Stimulate physics of this body part
	MeshComponent->SetSimulatePhysics(true);
}

// Called every frame
void AEnemySword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

