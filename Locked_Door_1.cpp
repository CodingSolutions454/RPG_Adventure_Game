// Fill out your copyright notice in the Description page of Project Settings.


#include "Locked_Door_1.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ALocked_Door_1::ALocked_Door_1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create this box collision
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    BoxComponent->InitBoxExtent(FVector(150, 150, 150));
    RootComponent = BoxComponent;

    // Create this mesh
    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Door->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
    Door->SetupAttachment(RootComponent);

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALocked_Door_1::OnOverlapBegin);
}

void ALocked_Door_1::BeginPlay()
{

}

// Called every frame
void ALocked_Door_1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// This actors collsions
void ALocked_Door_1::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Key_Collision_1")) 
    {
        // Calls this logic to rotate the mesh
        UnlockDoor();
        
        OtherActor->Destroy();
    }
}

// Rotate this actor 45 degrees
void ALocked_Door_1::UnlockDoor()
{
    FRotator SetThisRotation = GetActorRotation();
    SetThisRotation.Yaw += 45.0f;
    SetActorRotation(SetThisRotation);
}
