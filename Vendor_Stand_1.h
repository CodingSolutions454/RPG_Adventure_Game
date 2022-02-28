// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vendor_Stand_1.generated.h"

UCLASS()
class MYPROJECT_API AVendor_Stand_1 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVendor_Stand_1();

	// Creates a mesh for this actor
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Stand;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
