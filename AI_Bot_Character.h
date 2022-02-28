// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/inputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Waypoint.h"
#include "EnemySword.h"
#include "Perception/AISightTargetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "AI_Bot_Character.generated.h"

class UPawnSensingComponent;

UCLASS()
class MYPROJECT_API AAI_Bot_Character : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_Bot_Character();

	/** Location of this charcaters spawn point. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* Sword_Hand_R;

	/** Location on MyBow mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* head;

	/** Arrow offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector headOffset;
	
	/** Sound to play each time we do the SwordAttack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* SwordAttackSound;

	// Waypoint reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AWaypoint* NextWaypoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AArrow* Arrow;
	
	// Is dead
	bool bDead;

	// Do one bool
	bool bDoOnce;

	// Health amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Power;

	// lose health amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Lose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Sword;

	// Health treshold
	UPROPERTY(EditAnywhere)
		float Power_Treshold;

	 // Lose health treshold
	UPROPERTY(EditAnywhere)
		float Lose_Treshold;

	// Set up for this actors basic collisions
	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);
	 
	// Head collisions
	UFUNCTION()
		void OnHeadCollision(class UPrimitiveComponent* HitComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);

	// Right body side collisions
	UFUNCTION()
		void OnRightBodyCollision(class UPrimitiveComponent* HitComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);

	// Left body side collisions
	UFUNCTION()
		void OnLeftBodyCollision(class UPrimitiveComponent* HitComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);

	// Right arm collisions
	UFUNCTION()
		void OnRightArmCollision(class UPrimitiveComponent* HitComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);

	// left arm collisions
	UFUNCTION()
		void OnLeftArmCollision(class UPrimitiveComponent* HitComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Create Pawnsensing component
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UPawnSensingComponent* PawnSensing;

	// For OnPawn Detected
	UFUNCTION()
		void SeePawn(APawn* Pawn);

	// Enemy sword class refernce
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemySword> Enemy1class;
	class AEnemySword* EnemySword;

	// Body part head class refernce
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAI_Bot_Character_Head> BodyPart1class;
	class AAI_Bot_Character_Head* AI_Bot_Character_Head;

	// Body part headless body class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAI_Bot_Character_HeadlessBody> BodyPart2class;
	class AAI_Bot_Character_HeadlessBody* AI_Bot_Character_HeadlessBody;

	// Upper right body part class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAI_Bot_Character_RightUpBody> BodyPart3class;
	class AAI_Bot_Character_RightUpBody* AI_Bot_Character_RightUpBody;

	// Lower right body part class refernce
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAI_Bot_Character_RightLowBody> BodyPart4class;
	class AAI_Bot_Character_RightLowBody* AI_Bot_Character_RightLowBody;

	// Upper left body part class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAI_Bot_Character_LeftUpBody> BodyPart5class;
	class AAI_Bot_Character_LeftUpBody* AI_Bot_Character_LeftUpBody;

	// Lower left body part class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAI_Bot_Character_LeftLowBody> BodyPart6class;
	class AAI_Bot_Character_LeftLowBody* AI_Bot_Character_LeftLowBody;

	// No left arm body part class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAI_Bot_Character_NoLeftArmBody> BodyPart7class;
	class AAI_Bot_Character_NoLeftArmBody* AI_Bot_Character_NoLeftArmBody;

	// Left Arm body part arm class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAI_Bot_Character_LeftArm> BodyPart8class;
	class AAI_Bot_Character_LeftArm* AI_Bot_Character_LeftArm;

	// No right arm body part reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAI_Bot_Character_NoRightArmBody> BodyPart9class;
	class AAI_Bot_Character_NoRightArmBody* AI_Bot_Character_NoRightArmBody;

	// Right arm body part class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AAI_Bot_Character_RightArm> BodyPart10class;
	class AAI_Bot_Character_RightArm* AI_Bot_Character_RightArm;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Head collision box
	UPROPERTY(EditAnywhere)
		class UBoxComponent* HeadCollision;

	// Right body collision
	UPROPERTY(EditAnywhere)
		class UBoxComponent* RightBodyCollision;

	// Left body collision
	UPROPERTY(EditAnywhere)
		class UBoxComponent* LeftBodyCollision;

	// Right arm collision
	UPROPERTY(EditAnywhere)
		class UBoxComponent* RightArmCollision;

	// Left arm collision
	UPROPERTY(EditAnywhere)
		class UBoxComponent* LeftArmCollision;

	// AI seeing is attached this players head
	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Implement The Generic Team Interface 
	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const;

	void SwordAttack();
	// Set up for my characters Equipped sword animations, with weapon switching sockets.
	void EquippedSword();
	// Set up for my characters Unequipped sword animations, with weapon switching sockets.
	void UnequippedSword();

	// So that my character can animate.
	UPROPERTY(EditAnywhere, Category = "Animation")
		AActor* AI_Bot_Character;

	// Equipped sword animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimSequence* Equipped_Sword;

	// Unequipped sword animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimSequence* Unequipped_Sword;

	// Sword attack 1 animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Sword_Attack_1;
};

