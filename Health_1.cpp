// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Health_1.h"

// Sets default values
AHealth_1::AHealth_1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates a body for this Arrow.
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickUp");
	RootComponent = PickUpMesh;

	// Create a collision box for this actor
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentHit.AddDynamic(this, &AHealth_1::OnHit);
}

void AHealth_1::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

// Called when the game starts or when spawned
void AHealth_1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealth_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}