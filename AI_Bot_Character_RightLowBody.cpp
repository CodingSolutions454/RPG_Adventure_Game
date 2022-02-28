// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot_Character_RightLowBody.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AAI_Bot_Character_RightLowBody::AAI_Bot_Character_RightLowBody()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Body part mesh
	BodyPart = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = BodyPart;
}

// Called when the game starts or when spawned
void AAI_Bot_Character_RightLowBody::BeginPlay()
{
	Super::BeginPlay();
	
	// Stimulate physics of this body part
	BodyPart->SetSimulatePhysics(true);
}

// Called every frame
void AAI_Bot_Character_RightLowBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}