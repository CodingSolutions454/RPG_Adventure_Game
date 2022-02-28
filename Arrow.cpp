// Fill out your copyright notice in the Description page of Project Settings.

#include "Arrow.h"
#include "AI_Bot_Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "ParticleDefinitions.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AArrow::AArrow()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Creates a body for this Arrow.
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;

	// Use a ProjectileMovementComponent to govern this projectile's movement.
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));

	// Create a collision box for this actor
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentHit.AddDynamic(this, &AArrow::OnHit);
	
	ProjectileMovement->InitialSpeed = 40000.f;
	ProjectileMovement->MaxSpeed = 40000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Location of the head socket
	headOffset = FVector(100.0f, 0.0f, 10.0f);
	
	// Destroy after 20 seconds by default
	InitialLifeSpan = 20.0f;
}

// This actors collisions with other actors
void AArrow::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("E1"))
	{
		Destroy();
	}
}

