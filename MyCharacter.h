// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/inputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "MySword.h"
#include "MyBow.h"
#include "Arrow_Set.h"
#include "Quiver.h"
#include "Blueprint/UserWidget.h"
#include "GenericTeamAgentInterface.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter(const FObjectInitializer& ObjectInitialize);

	// Our character items widget
	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Items_Widget_Class;
	UUserWidget* Player_Items_Widget;

	// Our vendor widget items to buy for our character
	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Vendor_1_Widget_Class;
	UUserWidget* Player_Vendor_1_Widget;

	// Slow time effect widget
	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Slow_Time_Effect_Widget_Class;
	UUserWidget* Player_Slow_Time_Effect_Widget;

	/** Location of this charcaters spawn point. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* Sword_Hand_R;

	// Third person CameraBoom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* ThirdPersonCameraBoom;

	//Third person camerea
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* ThirdPersonCamera;

	// First person CameraBoom
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		USpringArmComponent* FirstPersonCameraBoom;

	// First Person Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		UCameraComponent* FirstPersonCamera;

	// Movement Axis
	void MoveForward(float Axis);
	void MoveRight(float Axis);

	// Bool for if this charcater can die
	bool bDead;

	// First person camera is active switch
	bool FirstPersonCamSwitched;

	// Third person camera is active switch
	bool ThirdPersonCamSwitched;

	// Does our character have any keys?
	bool WeOurHoldingDoorKey1;

	// Is hanging on ledge (Climbing)
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool IsHangingOnLedge;

	// Is healing our character
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool IsHealing;

	// Is Stamina regening our character
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool IsStaminaRegen;

	// Is food regening our character
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool IsFoodRegen;

	// Is water regening our character
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool IsWaterRegen;

	// Is buying from vendor
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool IsBuyingHealth;

	// Is buying from vendor
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool IsBuyingStamina;

	// Is buying from vendor
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool IsBuyingFood;

	// Is buying from vendor
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool IsBuyingWater;

	// Is game saved
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool GameSaved;

	// Is game loaded
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool GameLoaded;

	// Is our character colliding with the key lock?
	bool IsCollidingWithKeyLock;

	// Is in slow motion?
	bool IsInSlowMotion;

	// Is blinkink?
	bool IsBlinking;

	// Widegt remover bool
	bool WidgetRemover;

	// Are we in front of the vendor
	bool IsNextToVendor;

	// Is the game paused
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	bool IsGamePaused;

	// Is the character being hit
	bool IsBeingHit;

	// Health amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Health;

	// Stamina amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Stamina;

	// Hunger amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Hunger;

	// Thirst amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Thirst;

	// Arrows amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Arrows;

	// currency amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Currency;

	// keys amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Key;

	// Blink energy amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float BlinkEnergy;

	// Slow motion amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float SlowTimeEnergy;

	// This actors blink distance
	   float BlinkingDistance = 3000;

	// Amount of these items we have
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	    float HealthPickUpAmount;

	// Amount of these items we have
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float StaminaPickUpAmount;

	// Amount of these items we have
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float FoodPickUpAmount;
	
	// Amount of these items we have
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float WaterPickUpAmount;

	// Amount of these items we have
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float PauseValue;
	
	// Currency treshold
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Currency_Treshold;

	// Arrows treshold
	UPROPERTY(EditAnywhere)
		float Arrows_Treshold;

	// Health treshold
	UPROPERTY(EditAnywhere)
		float Health_Treshold;

	// Stamina treshold
	UPROPERTY(EditAnywhere)
		float Stamina_Treshold;

	// Hunger treshold
	UPROPERTY(EditAnywhere)
		float Hunger_Treshold;

	// thirst treshold
	UPROPERTY(EditAnywhere)
		float Thirst_Treshold;

	// Key treshold
	UPROPERTY(EditAnywhere)
		float Key_Treshold;

	// Blink energy treshold
	UPROPERTY(EditAnywhere)
		float BlinkEnergy_Treshold;

	// Slow time energy treshold
	UPROPERTY(EditAnywhere)
		float SlowTimeEnergy_Treshold;

	// Set up for our character collisions.
	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Restart this game if the charcters health is less then 0
	void RestartGame();

	// Use engine particle system to create a Spark
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* BlinkEffectSystem;
	class UParticleSystemComponent* CreateBlinkEffect(class UParticleSystem* BlinkEffect);

	// Get the player controller for our character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class APlayerController* PlayerController;

	// Heal the character on clicked event
	void Healed();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// MySword class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AMySword> Swordclass;
	class AMySword* MySword;

	// MyBow class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AMyBow> Bowclass;
	class AMyBow* MyBow;

	// Arrow set class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AArrow_Set> Setclass;
	class AArrow_Set* Arrow_Set;

	// Quiver Set class reference
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AQuiver> Quiverclass;
	class AQuiver* Quiver;

	/** Arrow class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AArrow> Arrowclass;

	/** Key class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AKey_Collision_1> Key1class;

	/** Can launched this character */
	UFUNCTION()
		virtual void Blink(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Sound to play each time we do the BowAttack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* BowAttackSound;

	/** Sound to play each time we do the SwordAttack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* SwordAttackSound;

	/** Sound to play each time we do the blink */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* BlinkSound;

	/** Arrow offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector Sword_Hand_ROffset;

private:
	// Can Save game.
	void SaveGame();
	
	// Can Load game.
	void LoadGame();
	
	// Bow attack 
	void BowAttack();
	
	// Sword attack
	void SwordAttack();
	
	// equipped sword
	void EquippedSword();
	
	// Unequiped sword
	void UnequippedSword();
	
	// Use key
	void UseKey();

	// Slow motion ability
	void SlowMotionAbility();

	// Remove slow motion ability
	void RemoveSlowMotion();

	// Remove the blink ability
	void RemoveBlinkEffects();

	// Can pause game
	void PauseMenu();

	// Can unpause game
	void RemovePauseMenu();

	// Time dialation
	const float TimeDialation = 0.1f;

	// Use time manger to handle the slow motion ability
	FTimerHandle SlowTimeHandle;

	// Use time manger to handle the pause game
	FTimerHandle PauseGame;

	// Use time manger to handle the pause game
	FTimerHandle UnPauseGame;

	// Set third person camera active
	void ThirdPersonCamActive();

	// Set first person camera active
	void FirstPersonCamActive();

	// Do blink
	void Blinking();

	// So that our character can animate.
	UPROPERTY(EditAnywhere, Category = "Animation")
		AActor* MyCharacter;

	// Equipped sword animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Equipped_Sword;

	// Unequipped sword animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Unequipped_Sword;

	// Sword attack 1 animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Sword_Attack_1;

	// Set up for Bow attack 1 animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Bow_Attack_1;

	// PickUp animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* PickUpItem;

	// Open door animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* UnlockDoorAnimation;
};