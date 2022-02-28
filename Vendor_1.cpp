// Fill out your copyright notice in the Description page of Project Settings.


#include "Vendor_1.h"
#include "Components/BoxComponent.h"

// Sets default values
AVendor_1::AVendor_1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create this box collision
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    BoxComponent->InitBoxExtent(FVector(150, 150, 150));
    RootComponent = BoxComponent;

    // Create this mesh
    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Door->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
    Door->SetupAttachment(RootComponent);

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AVendor_1::OnOverlapBegin);
}

void AVendor_1::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    
}

// Called when the game starts or when spawned
void AVendor_1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVendor_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
