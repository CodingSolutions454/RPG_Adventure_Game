// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MySword.generated.h"

UCLASS()
class MYPROJECT_API AMySword : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMySword();

	// Creates a body for this mesh
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* MeshComponent;

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
