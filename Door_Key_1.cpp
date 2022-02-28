// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Door_Key_1.h"

// Sets default values
ADoor_Key_1::ADoor_Key_1()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates a body for this Arrow.
	DoorKeyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Key");
	RootComponent = DoorKeyMesh;

	// Create a collision box for this actor
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentHit.AddDynamic(this, &ADoor_Key_1::OnHit);
}

void ADoor_Key_1::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

// Called when the game starts or when spawned
void ADoor_Key_1::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADoor_Key_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

