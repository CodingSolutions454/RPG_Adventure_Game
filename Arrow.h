// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class MYPROJECT_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	
	// Sets default values for this actor's properties
	AArrow();

	// Creates a body for this actor
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* MeshComponent;
	
	// Creates a collison box
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;

	/** Location on MyBow mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* head;

	/** offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector headOffset;
	
	// This actors collisions
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
