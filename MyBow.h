// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyBow.generated.h"

UCLASS()
class MYPROJECT_API AMyBow : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyBow();

	// Creates a body for this actor
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* MeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
