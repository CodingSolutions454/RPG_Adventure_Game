// Fill out your copyright notice in the Description page of Project Settings.


#include "Quiver.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AQuiver::AQuiver()
{
	// Creates a body for this actor
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;
}


