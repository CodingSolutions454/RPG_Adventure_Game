// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UMySaveGame();

	// Grt this actors last saved location
	UPROPERTY(EditAnyWhere)
		FVector PlayerLocation;

	// Amount of these items we have
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float HealthPickUpAmount;
};
