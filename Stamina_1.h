// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stamina_1.generated.h"

UCLASS()
class MYPROJECT_API AStamina_1 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStamina_1();

	// Create static mesh
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* PickUpMesh;

	// Create this collision box
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;

	// This actors collisions
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
