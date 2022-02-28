// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "InGameUI.h"
#include "Animation/AnimInstance.h"
#include "AI_Bot_Character.h"
#include "Engine/World.h"
#include "GenericTeamAgentInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "MySword.h"
#include "MyBow.h"
#include "EnemySword.h"
#include "Arrow.h"
#include "Arrow_Set.h"
#include "Quiver.h"
#include "MySaveGame.h"
#include "Key_Collision_1.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AMyCharacter::AMyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This is the team my character is in and its different to the enemy team. It so I am identified as an enemy my the enemy team.
	{
		SetGenericTeamId(FGenericTeamId(10));
	}

	// Capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 82.0f);

	// Set this actors rotation values
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character movement values
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Set up the camera boom attachment to our player
	ThirdPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom1"));
	ThirdPersonCameraBoom->SetupAttachment(RootComponent);
	ThirdPersonCameraBoom->TargetArmLength = 300.0f;
	ThirdPersonCameraBoom->bUsePawnControlRotation = true;

	// Create this camera 
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera1"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonCameraBoom, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	// Set up the camera boom attachment to our player
	FirstPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom2"));
	FirstPersonCameraBoom->SetupAttachment(GetMesh(), FName("head"));
	FirstPersonCameraBoom->TargetArmLength = 300.0f;
	FirstPersonCameraBoom->bUsePawnControlRotation = true;

	// Create this camera 
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera2"));
	FirstPersonCamera->SetupAttachment(FirstPersonCameraBoom, USpringArmComponent::SocketName);
	FirstPersonCamera->SetupAttachment(GetMesh(), FName("head"));
	FirstPersonCamera->bUsePawnControlRotation = true;

	// bool conditions
	bDead = false;
	FirstPersonCamSwitched = false;
	ThirdPersonCamSwitched = false;
	
	IsHangingOnLedge = false;
	WeOurHoldingDoorKey1 = false;
	IsCollidingWithKeyLock = false;
	
	IsInSlowMotion = false;
	IsBlinking = false;
	IsNextToVendor = false;
	WidgetRemover = false;
	
	IsGamePaused = false;
	IsBeingHit = false;
	
	IsHealing = false;
	IsStaminaRegen = false;
	IsFoodRegen = false;
	IsWaterRegen = false;
	
	IsBuyingHealth = false;
	IsBuyingStamina = false;
	IsBuyingFood = false;
	IsBuyingWater = false;

	GameSaved = false;
	GameLoaded = false;
	
	// Our spawn location for our projectile spawn
	Sword_Hand_ROffset = FVector(100.0f, 0.0f, 10.0f);

	// Float values
	Arrows = 200;
	Health = 50.0f;
	Currency = 500.0f;
	Stamina = 100.0f;
	Hunger = 100.0f;
	Thirst = 100.0f;
	Key = 0.0f;
	BlinkEnergy = 100.0f;
	SlowTimeEnergy = 100.0f;
	HealthPickUpAmount = 0.0f;
	StaminaPickUpAmount = 0.0f;
	FoodPickUpAmount = 0.0f;
	WaterPickUpAmount = 0.0f;
	PauseValue = 1.0f;
}

// Particle effect for our blink ability
UParticleSystemComponent* AMyCharacter::CreateBlinkEffect(UParticleSystem* BlinkEffect)
{
	class UParticleSystemComponent* retVal = NULL;

	if (BlinkEffect)
		{
		class UWorld* const world = GetWorld();

		if (world)
		{
			FVector myPos = GetActorLocation();
			FRotator myRot = GetActorRotation();

			retVal = UGameplayStatics::SpawnEmitterAtLocation(world, BlinkEffect, myPos, myRot, true);
		}
	}

	return retVal;
}

// The bool event for this function can be called from a widget button as its bool public. Player will be healed and health set to 100 which is set in tick.
void AMyCharacter::Healed()
{
	IsHealing = true;
    
	if (IsHealing == true) 
	{
		HealthPickUpAmount -= 1.0f;
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Our capsule component for are BeginOnOverlap event
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnBeginOverLap);

	// Attach our items to our character on spawn
	FActorSpawnParameters SpawnParems;
	SpawnParems.bNoFail = true;
	SpawnParems.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Attach our sword to the chosen bone
	if (Swordclass)
	{
		FTransform MySwordTransform;

		MySwordTransform.SetLocation(FVector::ZeroVector);
		MySwordTransform.SetRotation(FQuat(FRotator::ZeroRotator));
		
		MySword = GetWorld()->SpawnActor<AMySword>(Swordclass, MySwordTransform, SpawnParems);
		if (MySword)
		{
			MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Thigh_Sword_L"));
		}
	}

	SpawnParems.bNoFail = true;
	SpawnParems.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Attach our bow to the chosen bone
	if (Bowclass)
	{
		FTransform MyBowTransform;

		MyBowTransform.SetLocation(FVector::ZeroVector);
		MyBowTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		MyBow = GetWorld()->SpawnActor<AMyBow>(Bowclass, MyBowTransform, SpawnParems);
		if (MyBow)
		{
			MyBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spine_Bow"));
		}
	}

	SpawnParems.bNoFail = true;
	SpawnParems.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Attach setclass to the chosen bone
	if (Setclass)
	{
		FTransform Arrow_SetTransform;

		Arrow_SetTransform.SetLocation(FVector::ZeroVector);
		Arrow_SetTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		Arrow_Set = GetWorld()->SpawnActor<AArrow_Set>(Setclass, Arrow_SetTransform, SpawnParems);
		if (Arrow_Set)
		{
			Arrow_Set->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spine_Arrow_Set"));
		}
	}

	SpawnParems.bNoFail = true;
	SpawnParems.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Attach quiver class to the chosen bone
	if (Quiverclass)
	{
		FTransform QuiverTransform;

		QuiverTransform.SetLocation(FVector::ZeroVector);
		QuiverTransform.SetRotation(FQuat(FRotator::ZeroRotator));
		
		Quiver = GetWorld()->SpawnActor<AQuiver>(Quiverclass, QuiverTransform, SpawnParems);
		if (Quiver)
		{
			Quiver->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spine_Quiver"));
		}
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Controller rotation yaw is constant if a projectile is fired
	bUseControllerRotationYaw = false;

	// Subtract hunger amount at this value over time
	Hunger -= Hunger_Treshold = 0.0001;

	// Subtract thirst amount at this value over time
	Thirst -= Thirst_Treshold = 0.0001;

	// Add stamina amount at this value over time
	Stamina += Stamina_Treshold = .1;

	// Recharge this actors stamina at this value back wards if it ticks over a 100
	if (Stamina > 100) 
	{

		Stamina -= Stamina_Treshold = .1;
	}

	// If our stamina is less then 100 the set it to recharge over time
	if (Stamina < 100)
	{
		Stamina += Stamina_Treshold = .1;
	}

	// If our stamina is less then 1 then start to reduce the players health. We do this for everytime we lose stamina and are stamina is less then 1
	if (Stamina < 1)
	{
		Health -= Health_Treshold = .1;
	}

	// Recharge this actors BlinkEnergy  backwards at this value over time
	if (BlinkEnergy > 100) 
	{

		BlinkEnergy -= BlinkEnergy_Treshold = .1;
	}

	// If our BlinkEnergy is less then 100 then set it to recharge over time
	if (BlinkEnergy < 100)
	{
		BlinkEnergy += BlinkEnergy_Treshold = .1;
	}
	
	// Create this particle effect for everytime we blink
	if (IsBlinking == true)
	{
		UParticleSystemComponent* BlinkEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BlinkEffectSystem, GetActorTransform());
	}
	
	// If our SlowTimeEnergy is less then 1 then set it to recharge at this value over time
	if (SlowTimeEnergy < 1)
	{
		RemoveSlowMotion();
		SlowTimeEnergy += SlowTimeEnergy_Treshold = .2;
	}

	// If our SlowTimeEnergy is more then 100 then set it to recharge backwards over time
	if (SlowTimeEnergy > 100)
	{
		SlowTimeEnergy -= SlowTimeEnergy_Treshold = .1;
	}

	// If we are using the slow motion ability then start subtracting from its value
	if (IsInSlowMotion == true) 
	{
		SlowTimeEnergy -= SlowTimeEnergy_Treshold = .1;
	}
	
	// is we are not using the slow motion ability them set it to recharge
	if (IsInSlowMotion == false)
	{
		SlowTimeEnergy += SlowTimeEnergy_Treshold = .1;
	}

	// Remove this effect if the value is less then 1
	if (SlowTimeEnergy < 1)
	{
		Player_Slow_Time_Effect_Widget->RemoveFromViewport();
	}

	// We can heal our chararcter by calling this bool condition from a widget button or input
	if (IsHealing && HealthPickUpAmount > 0) 
	{
		Health += 100.0f;

		HealthPickUpAmount -= 1.0f;

		IsHealing = false;
	}

	// We can regen our stamina by calling this bool condition from a widget button or input
	if (IsStaminaRegen && StaminaPickUpAmount > 0)
	{
		Stamina += 100.0f;

		StaminaPickUpAmount -= 1.0f;

		IsStaminaRegen = false;
	}

	// We can regen our food by calling this bool condition from a widget button or input
	if (IsFoodRegen && FoodPickUpAmount > 0)
	{
		Hunger += 100.0f;

		FoodPickUpAmount -= 1.0f;

		IsFoodRegen = false;
	}

	// We can regen our food by calling this bool condition from a widget button or input
	if (IsWaterRegen && WaterPickUpAmount > 0)
	{
		Thirst += 100.0f;

		WaterPickUpAmount -= 1.0f;

		IsWaterRegen = false;
	}
	
	// We can purchace this item as long as our character has enough currency. This will add this item to our items and the bool can be called from a widget or input button
	if (IsBuyingHealth == true && Currency > 19)
	{
		HealthPickUpAmount += 1.f;
		Currency -= 20.0f;
		IsBuyingHealth = false;
	}
	
	// We can purchace this item as long as our character has enough currency. This will add this item to our items and the bool can be called from a widget or input button
	if (IsBuyingStamina == true && Currency > 19)
	{
		StaminaPickUpAmount += 1.f;
		Currency -= 20.0f;
		IsBuyingStamina = false;
	}

	// We can purchace this item as long as our character has enough currency. This will add this item to our items and the bool can be called from a widget or input button
	if (IsBuyingFood == true && Currency > 19)
	{
		FoodPickUpAmount += 1.f;
		Currency -= 20.0f;
		IsBuyingFood = false;
	}

	// We can purchace this item as long as our character has enough currency. This will add this item to our items and the bool can be called from a widget or input button
	if (IsBuyingWater == true && Currency > 19)
	{
		WaterPickUpAmount += 1.f;
		Currency -= 20.0f;
		IsBuyingWater = false;

	}

	// If we are saving our game. Then we call the save game function and do so once by setting game saved is false 
	if (GameSaved == true) 
	{
		SaveGame();
		GameSaved = false;
	}

	// If we are loading our game. Then we call the load game function and do so once by setting game loaded is false
	if (GameLoaded == true)
	{
		LoadGame();
		GameLoaded = false;
	}

	// If game is paused is false. Then remove the pause menu and do once.
	if (IsGamePaused == false) 
	{
		IsGamePaused = false;
		WidgetRemover = true;
		
		RemovePauseMenu();
		WidgetRemover = false;
	}

	// If our health is 0. Then simulate phisics and after 3 seconds restart the game
	if (Health < 0) {

		if (!bDead) {

			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMyCharacter::RestartGame, 3.0f, false);
		}
	}

	// If our hunger is 0. Then simulate phisics and after 3 seconds restart the game
	if (Hunger < 0) {

		if (!bDead) {

			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMyCharacter::RestartGame, 3.0f, false);
		}
	}

	// If our thirst is 0. Then simulate phisics and after 3 seconds restart the game
	if (Thirst < 0) {

		if (!bDead) {

			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMyCharacter::RestartGame, 3.0f, false);
		}
	}

	// If first person camera is active then set the controller yaw rotation to true
	if (FirstPersonCamSwitched == true)
	{
		bUseControllerRotationYaw = true;
	}

	// If the third person camera is active then set the controller yaw rotation to false. 
	if (ThirdPersonCamSwitched == true)
	{
		bUseControllerRotationYaw = false;
	}

	// If the third person camera is active and hanging on ledge is true. Then set the yaw rotaion to false
	if (ThirdPersonCamSwitched == true && IsHangingOnLedge == false)
	{
		bUseControllerRotationYaw = false;
	}

	// If the third person camera is active and hanging on ledge is false. Then set the yaw rotaion to true
	if (ThirdPersonCamSwitched == true && IsHangingOnLedge == true)
	{
		bUseControllerRotationYaw = false;
	}

	// If the first person camera is true and hanging on ledge is false. Then set the yaw rotatioon to true
	if (FirstPersonCamSwitched == true && IsHangingOnLedge == false)
	{
		bUseControllerRotationYaw = true;
	}

	// If the first person cam is active and hanging on ledge is true. Set all rotaion to false. We then switch to the first person camera for the purpose of our character climbing
	if (FirstPersonCamSwitched == true && IsHangingOnLedge == true)
	{
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;

		FirstPersonCamSwitched = false;
		ThirdPersonCamSwitched = true;
		ThirdPersonCamera->SetActive(true);
		FirstPersonCamera->SetActive(false);
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	InputComponent->BindAction("Save", IE_Pressed, this, &AMyCharacter::SaveGame);
	InputComponent->BindAction("Load", IE_Pressed, this, &AMyCharacter::LoadGame);
	InputComponent->BindAction("EquippedSword", IE_Pressed, this, &AMyCharacter::EquippedSword);
	InputComponent->BindAction("UnequippedSword", IE_Pressed, this, &AMyCharacter::UnequippedSword);
	InputComponent->BindAction("SwordAttack", IE_Pressed, this, &AMyCharacter::SwordAttack);
	InputComponent->BindAction("BowAttack", IE_Pressed, this, &AMyCharacter::BowAttack);
	InputComponent->BindAction("ThirdPersonCamActive", IE_Pressed, this, &AMyCharacter::ThirdPersonCamActive);
	InputComponent->BindAction("FirstPersonCamActive", IE_Pressed, this, &AMyCharacter::FirstPersonCamActive);
	InputComponent->BindAction("SlowMotionAbility", IE_Pressed, this, &AMyCharacter::SlowMotionAbility);
	InputComponent->BindAction("Blinking", IE_Pressed, this, &AMyCharacter::Blinking);
	InputComponent->BindAction("UseKey", IE_Pressed, this, &AMyCharacter::UseKey);
	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AMyCharacter::PauseMenu);
}

// If we save our game we save the actors last location through the bool. GameSaved.
void AMyCharacter::SaveGame()
{
	GameSaved = true;
	
	if (GameSaved == true) 
	{
		//Create an instance of our savegame class
		UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		//Set the save game instance location equal to players location
		SaveGameInstance->PlayerLocation = this->GetActorLocation();
		//Save the Gameinstance
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("My Slot"), 0);
		//Log message to show we have saved game
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Saved"));
	}
}

// If we load our game we get the actors last saved loaction. This function needs improvemnet and will be improved on in the next update
void AMyCharacter::LoadGame()
{
	GameLoaded = true;
	
	if (GameLoaded == true) 
	{
		UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		//Load the saved game in to our savegameinstance varible
		SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("My Slot", 0));
		//Set the players position from the saved game file
		this->SetActorLocation(SaveGameInstance->PlayerLocation);
		//Log message to show we have loaded game
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Loaded"));
	}	
}


// Our fire a projectile logic
void AMyCharacter::BowAttack()
{
	
	if (Arrows > 0)
	{
		// try and fire a Arrow
		if (Arrowclass != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{
					// Get spawn rotation
					const FRotator SpawnRotation = GetControlRotation();

					// Get spawn location
					const FVector SpawnLocation = ((Sword_Hand_R != nullptr) ? Sword_Hand_R->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(Sword_Hand_ROffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the Arrow at the Bow.
					World->SpawnActor<AArrow>(Arrowclass, SpawnLocation, SpawnRotation, ActorSpawnParams);

					// try and play the sound if specified
					if (BowAttackSound != NULL)
					{
						UGameplayStatics::PlaySoundAtLocation(this, BowAttackSound, GetActorLocation());
					}

					// If we our in slow motion and do the bow attack. Then for that instance slow motion is removed for a small time frame
					if (IsInSlowMotion == true) 
					{
						RemoveSlowMotion();

						SlowTimeEnergy -= 4.0f;
						
						// The amount of time slow motion is removed
						FTimerHandle UnusedHandle;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMyCharacter::SlowMotionAbility, .20f, false);
					}

					// Plays animation for BowAttack 1
					GetMesh()->SetAnimation(Bow_Attack_1);
					GetMesh()->GetAnimInstance()->Montage_Play(Bow_Attack_1);

					// When the projectile is fired. Attach the bow to our default location
					MyBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("BowAttack_Hand_L"));

					// When the projectile is fired. Attach the sword to its default location.
					MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Thigh_Sword_L"));

					// Rotate the character in the direction that the projectile is fired
					bUseControllerRotationYaw = true;

					// We subtract 1 arrow for each time we fire this projectile
					Arrows -= 1.0f;
				}
			}
		}
	}
}

// Our sword attack logic
void AMyCharacter::SwordAttack()
{
	// Play animation for sword attack
	GetMesh()->SetAnimation(Sword_Attack_1);
	GetMesh()->GetAnimInstance()->Montage_Play(Sword_Attack_1);

	// When the sword attack happens. Attach the sword to the hand of our player
	MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Sword_Hand_R"));

	// When the sword attach happens. Attach the bow to its default location
	MyBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spine_Bow"));

	// When the sword attck happens. remove this amount of stamina
	if (Sword_Attack_1, true)
	{
	   Stamina -= Stamina_Treshold = 20;
	}

	// try and play the sound if specified
	if (SwordAttackSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SwordAttackSound, GetActorLocation());
	}

	// If we our in slow motion and do the sword attack. Then for that instance slow motion is removed for a small time frame
	if (IsInSlowMotion == true)
	{
		RemoveSlowMotion();

		SlowTimeEnergy -= 4.0f;
		
		// The amount of time slow motion is removed
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMyCharacter::SlowMotionAbility, .30f, false);
	}

}

// Our equipped sword logic
void AMyCharacter::EquippedSword()
{
	if (Equipped_Sword)
	{
		// Play equipped sword animation
		GetMesh()->SetAnimation(Equipped_Sword);
		GetMesh()->GetAnimInstance()->Montage_Play(Equipped_Sword);

		// When the equipped sword happens. Attach the Sword to our players hand
		MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Sword_Hand_R"));
	}
}

// Our unequipped sword logic
void AMyCharacter::UnequippedSword()
{
	if (Unequipped_Sword)
	{
		// Play unequipped sword animation
		GetMesh()->SetAnimation(Unequipped_Sword);
		GetMesh()->GetAnimInstance()->Montage_Play(Unequipped_Sword);

		// When the unequipped sword happens. Attach the Sword to our players hand
		MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Thigh_Sword_L"));
	}
}


// If we have at least one key and we are colliding with a door lock and we have pressed the key button. Then play the unlock door animation and remove one key from our character
void AMyCharacter::UseKey()
{
	// If we are colliding with the key lock and holding at least more then 1 key. Then we can create the collsion to open the locked door
	(IsCollidingWithKeyLock == true && WeOurHoldingDoorKey1 == true);
	{
		if (Key > 0)
		{
			// try and fire a Arrow
			if (Key1class != NULL)
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{

					{
						// Get spawn rotation
						const FRotator SpawnRotation = GetControlRotation();

						// Get spawn location
						const FVector SpawnLocation = ((Sword_Hand_R != nullptr) ? Sword_Hand_R->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(Sword_Hand_ROffset);

						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						// spawn the Arrow at the Bow.
						World->SpawnActor<AKey_Collision_1>(Key1class, SpawnLocation, SpawnRotation, ActorSpawnParams);


						// Rotate the character in the direction that the projectile is fired
						bUseControllerRotationYaw = true;

						// We subtract 1 key for each time we fire this projectile
						Key -= 1.0f;

						// Play unlock door animation
						GetMesh()->SetAnimation(UnlockDoorAnimation);
						GetMesh()->GetAnimInstance()->Montage_Play(UnlockDoorAnimation);
					}
				}
			}
		}
	}
}

// Slow motion ability
void AMyCharacter::SlowMotionAbility()
{
	// As long as we have more then 1 value of slow motion energy and slow motion is true
	if (SlowTimeEnergy > 1) 
	{
		IsInSlowMotion = true;
		
		// How long we use this ability
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDialation);
		GetWorldTimerManager().SetTimer(SlowTimeHandle, this, &AMyCharacter::RemoveSlowMotion, 2.f, false);

		if (SlowTimeEnergy > 99 && IsInSlowMotion == true)
		{
			// If we our in slow motion then add this effect to the viewport
			if (Player_Slow_Time_Effect_Widget_Class != nullptr) {

				Player_Slow_Time_Effect_Widget = CreateWidget(GetWorld(), Player_Slow_Time_Effect_Widget_Class);
				Player_Slow_Time_Effect_Widget->AddToViewport();

			}
		}
	}
}

// Remove slow motion after the timer has run out and slow motion is false
void AMyCharacter::RemoveSlowMotion()
{
	IsInSlowMotion = false;

	// Return to normal time speed
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GetWorldTimerManager().ClearTimer(SlowTimeHandle);
}

// Remove this effect using the time manager called in tick
void AMyCharacter::RemoveBlinkEffects()
{
	IsBlinking = false; 
}

// Pause game function
void AMyCharacter::PauseMenu()
{
	// Starting pause value = 1. 
	if (PauseValue == 1) 
	{
		IsGamePaused = true;
		
		// Show mouse curser if the game is paused
		if (IsGamePaused == true)
		{
			PlayerController = GetWorld()->GetFirstPlayerController();
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;

			// If the game is paused the we need to disable movemnet
			DisableInput(PlayerController);
			DisableInput(Cast<APlayerController>(MyCharacter));
			
			// The create the items menu.
			if (Player_Items_Widget_Class != nullptr)
			{
				Player_Items_Widget = CreateWidget(GetWorld(), Player_Items_Widget_Class);
				Player_Items_Widget->AddToViewport();
			}

			// If the game is paused and we are also colliding with the vendor. Create the vendor widegt to allow the player to purchase items from the vendor
			if (IsNextToVendor == true)
			{
				if (Player_Vendor_1_Widget_Class != nullptr)
				{
					WidgetRemover = true;
					Player_Vendor_1_Widget = CreateWidget(GetWorld(), Player_Vendor_1_Widget_Class);
					Player_Vendor_1_Widget->AddToViewport();
					IsNextToVendor = false;
				}
			}
		}

		// pause value is now 0 but added when we unpause. This prevents crashes
		PauseValue -= 1.0f;
	}
}

// Remove pause menu functionality. This is called in tick and set to do once.
void AMyCharacter::RemovePauseMenu()
{
	// Pause menu is always less then 1 at the start of the removing of the pause menu
	if (PauseValue == 0)
	{
		IsGamePaused = false;
		
		// If the game paused then we enable input again. Called in tick
		EnableInput(PlayerController);
		EnableInput(Cast<APlayerController>(PlayerController));
		EnableInput(Cast<APlayerController>(MyCharacter));

		// Remove the items widget from the screen
		Player_Items_Widget->RemoveFromParent();
		
		// If we have the vendor widegt open then this will also be removed. 
		if (Player_Vendor_1_Widget) 
		{
			Player_Vendor_1_Widget->RemoveFromViewport();
		}
		
		// Remove the mouse curer from view.
		PlayerController = GetWorld()->GetFirstPlayerController();
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
		
		// Then add a pause value of 1. This is so we can pause our game again as the pause function has this codition
		PauseValue += 1.0f;
	}
}

// Set the third person camera active
void AMyCharacter::ThirdPersonCamActive()
{
	FirstPersonCamSwitched = false;

	ThirdPersonCamSwitched = true;

	ThirdPersonCamera->SetActive(true);

	FirstPersonCamera->SetActive(false);
}

void AMyCharacter::Blink(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	// Launch character is declared in blink
}

// Set first person camera active
void AMyCharacter::FirstPersonCamActive()
{
	ThirdPersonCamSwitched = false;

	FirstPersonCamSwitched = true;

	FirstPersonCamera->SetActive(true);

	ThirdPersonCamera->SetActive(false);
}

// Blink ability
void AMyCharacter::Blinking()
{
	// We must have more the the value 0f 33 blink energy to do the blink and blinking is true
	if (BlinkEnergy > 33)
	{
		IsBlinking = true;

		bUseControllerRotationYaw = true;

		SlowTimeEnergy -= 4.0f;

		// If we our in slow motion and blink. Then for that instance slow motion is removed for a small time frame
		if (IsInSlowMotion == true)
		{
			RemoveSlowMotion();

			// The amount of time slow motion is removed
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMyCharacter::SlowMotionAbility, .20f, false);
		}

		// Remove this amount of blink energy for everytime we do the blink
		BlinkEnergy -= 33.0f;
		
		// Get our actors forward vector
		const FVector ForwardDir = this->GetActorRotation().Vector();

		// Get this actors distance
		LaunchCharacter(ForwardDir * BlinkingDistance, true, true);

		// try and play the sound if specified
		if (BlinkSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BlinkSound, GetActorLocation());
		}

		if (IsBlinking == true)
		{
			// The amount of time slow motion is removed
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMyCharacter::RemoveBlinkEffects, .10f, false);

		}
	}
}

// If our character is dead. Set these movement values to false
void AMyCharacter::MoveForward(float Axis)
{
	if (!bDead) {

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

// If our character is dead. Set these movement values to false
void AMyCharacter::MoveRight(float Axis) {

	if (!bDead) {

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

// Our characters collision with other actors
void AMyCharacter::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	
	if (OtherActor->ActorHasTag("Door_Key_1"))
	{
		WeOurHoldingDoorKey1 = true;

		// Add one door key value to our character
		Key += 1.0f;

		// Plays animation for PickUpItem.
		GetMesh()->SetAnimation(PickUpItem);
		GetMesh()->GetAnimInstance()->Montage_Play(PickUpItem);

		// Attach the sword to the hip of our player as we dont want it to get in the way of the above animation
		MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Thigh_Sword_L"));

		// Attach the bow to the spine of our player as we dont want it to get in the way of the above animation
		MyBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spine_Bow"));

		OtherActor->Destroy();
	}
	
	if (OtherActor->ActorHasTag("Health_Pickup"))
	{
		// Add one health pick up value to our character
		HealthPickUpAmount += 1.0f;

		// Plays animation for PickUpItem.
		GetMesh()->SetAnimation(PickUpItem);
		GetMesh()->GetAnimInstance()->Montage_Play(PickUpItem);

		// Attach the sword to the hip of our player as we dont want it to get in the way of the above animation
		MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Thigh_Sword_L"));

		// Attach the bow to the spine of our player as we dont want it to get in the way of the above animation
		MyBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spine_Bow"));

		OtherActor->Destroy();
	}
	
	if (OtherActor->ActorHasTag("Stamina_Pickup"))
	{
		// Add one Stamina pick up value to our character
		StaminaPickUpAmount += 1.0f;

		// Plays animation for PickUpItem.
		GetMesh()->SetAnimation(PickUpItem);
		GetMesh()->GetAnimInstance()->Montage_Play(PickUpItem);

		// Attach the sword to the hip of our player as we dont want it to get in the way of the above animation
		MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Thigh_Sword_L"));

		// Attach the bow to the spine of our player as we dont want it to get in the way of the above animation
		MyBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spine_Bow"));

		OtherActor->Destroy();
	}

	if (OtherActor->ActorHasTag("Food_Pickup"))
	{
		// Add one food pick up value to our character
		FoodPickUpAmount += 1.0f;

		// Plays animation for PickUpItem.
		GetMesh()->SetAnimation(PickUpItem);
		GetMesh()->GetAnimInstance()->Montage_Play(PickUpItem);

		// Attach the sword to the hip of our player as we dont want it to get in the way of the above animation
		MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Thigh_Sword_L"));

		// Attach the bow to the spine of our player as we dont want it to get in the way of the above animation
		MyBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spine_Bow"));

		OtherActor->Destroy();
	}

	if (OtherActor->ActorHasTag("Water_Pickup"))
	{
		// Add one food pick up value to our character
		WaterPickUpAmount += 1.0f;

		// Plays animation for PickUpItem.
		GetMesh()->SetAnimation(PickUpItem);
		GetMesh()->GetAnimInstance()->Montage_Play(PickUpItem);

		// Attach the sword to the hip of our player as we dont want it to get in the way of the above animation
		MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Thigh_Sword_L"));

		// Attach the bow to the spine of our player as we dont want it to get in the way of the above animation
		MyBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Spine_Bow"));

		OtherActor->Destroy();
	}
	
	// Other collions from other actors
	if (OtherActor->ActorHasTag("EnemySword"))
	{
		Health -= 05.0f;

		if (Health > 100.0f)
			Health = 100.0f;

	}

	if (OtherActor->ActorHasTag("Food_Pickup")) {


		Hunger += 50.0f;

		if (Hunger > 100.0f)
			Hunger = 100.0f;

		OtherActor->Destroy();
	}

	if (OtherActor->ActorHasTag("Water_Pickup")) {

		Thirst += 50.0f;

		if (Thirst > 100.0f)
			Thirst = 100.0f;

		OtherActor->Destroy();
	}

	if (OtherActor->ActorHasTag("Arrow_1_Pickup")) {

		Arrows += 1.0f;

		OtherActor->Destroy();
	}

	if (OtherActor->ActorHasTag("DoorLock"))
	{
		IsCollidingWithKeyLock = true;
	}
	
	if (OtherActor->ActorHasTag("Vendor_1"))
	{
		IsNextToVendor = true;
	}
}

void AMyCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		UE_LOG(LogTemp, Warning, TEXT("We Ended"));
		
	}
}

// If the game is restarting then open the current level at our characters spawn
void AMyCharacter::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}