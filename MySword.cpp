// Fill out your copyright notice in the Description page of Project Settings.


#include "MySword.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AMySword::AMySword()
{
	// Create a boy for this actor
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;
}

void AMySword::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//If this actor has collision with laser1 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("AI_Bot_Character")) {

	}
}

// Called when the game starts or when spawned
void AMySword::BeginPlay()
{
	Super::BeginPlay();

}

// Called to bind functionality to input
void AMySword::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
