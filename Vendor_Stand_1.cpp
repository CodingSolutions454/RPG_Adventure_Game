// Fill out your copyright notice in the Description page of Project Settings.


#include "Vendor_Stand_1.h"

// Sets default values
AVendor_Stand_1::AVendor_Stand_1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create this mesh
	Stand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Stand->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVendor_Stand_1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVendor_Stand_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

