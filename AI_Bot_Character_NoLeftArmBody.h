// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AI_Bot_Character_NoLeftArmBody.generated.h"

UCLASS()
class MYPROJECT_API AAI_Bot_Character_NoLeftArmBody : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAI_Bot_Character_NoLeftArmBody();

	// Actor body part
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* BodyPart;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
