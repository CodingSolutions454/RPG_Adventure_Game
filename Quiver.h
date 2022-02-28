// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quiver.generated.h"

UCLASS()
class MYPROJECT_API AQuiver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuiver();

	// Creates a body for this actor
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* MeshComponent;

};
