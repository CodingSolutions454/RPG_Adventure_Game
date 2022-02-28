// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Vendor_1.generated.h"

UCLASS()
class MYPROJECT_API AVendor_1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVendor_1();

	// Creates a mesh for this actor
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Door;

	// Creates collision box for this actor
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
