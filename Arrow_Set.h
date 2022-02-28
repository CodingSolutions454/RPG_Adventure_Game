// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow_Set.generated.h"

UCLASS()
class MYPROJECT_API AArrow_Set : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow_Set();


	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* MeshComponent;

};
