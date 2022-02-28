// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Waypoint.generated.h"

UCLASS()
class MYPROJECT_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaypoint();
	
	// Scene component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* Root;

	// Box component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;

	// Waypoint for actor using this waypoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AWaypoint* NextWaypoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Collision event for when player enters
	UFUNCTION()
		void OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
