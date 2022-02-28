// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBow.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AMyBow::AMyBow()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;

}

// Called when the game starts or when spawned
void AMyBow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AMyBow::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

