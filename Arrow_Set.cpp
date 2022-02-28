// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow_Set.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AArrow_Set::AArrow_Set()
{
 	

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;
}


