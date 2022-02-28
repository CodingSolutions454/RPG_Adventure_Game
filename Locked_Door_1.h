// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Locked_Door_1.generated.h"

UCLASS()
class MYPROJECT_API ALocked_Door_1 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALocked_Door_1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Creates a mesh for this actor
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Door;

	// Creates collision box for this actor
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	void UnlockDoor();
};
