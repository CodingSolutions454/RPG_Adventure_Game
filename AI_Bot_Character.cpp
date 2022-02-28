// Fill out your copyright notice in the Description page of Project Settings

#include "AI_Bot_Character.h"
#include "Animation/AnimInstance.h"
#include "AI_Bot_Character_Head.h"
#include "AI_Bot_Character_HeadlessBody.h"
#include "AI_Bot_Character_RightUpBody.h"
#include "AI_Bot_Character_RightLowBody.h"
#include "AI_Bot_Character_LeftUpBody.h"
#include "AI_Bot_Character_LeftLowBody.h"
#include "AI_Bot_Character_NoRightArmBody.h"
#include "AI_Bot_Character_NoLeftArmBody.h"
#include "AI_Bot_Character_RightArm.h"
#include "AI_Bot_Character_LeftArm.h"
#include "Arrow.h"
#include "MyCharacter.h"
#include "EnemySword.h"
#include "GenericTeamAgentInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAI_Bot_Character::AAI_Bot_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	{
		SetGenericTeamId(FGenericTeamId(5));
	}

	// Set capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 82.0f);

	// Set plaYers rotations
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character Movement values
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	
	// Create the head collsion box
	HeadCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Head"));
	HeadCollision->SetupAttachment(GetRootComponent());
	HeadCollision->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Character::OnHeadCollision);

	// Create right body collision box
	RightBodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightBody"));
	RightBodyCollision->SetupAttachment(GetRootComponent());
	RightBodyCollision->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Character::OnRightBodyCollision);

	// Create Left Body collision
	LeftBodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftBody"));
	LeftBodyCollision->SetupAttachment(GetRootComponent());
	LeftBodyCollision->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Character::OnLeftBodyCollision);

	// Create a right arm collision
	RightArmCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightArm"));
	RightArmCollision->SetupAttachment(GetRootComponent());
	RightArmCollision->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Character::OnRightArmCollision);

	// Create a left arm Collision
	LeftArmCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftArm"));
	LeftArmCollision->SetupAttachment(GetRootComponent());
	LeftArmCollision->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Character::OnLeftArmCollision);

	// Location of the head socket
	headOffset = FVector(100.0f, 0.0f, 10.0f);

	// boolS
	bDead = false;
	bDoOnce = false;
	
	// floats
	Power = 100.0f;

	// This characters walk speed
	GetCharacterMovement()->MaxWalkSpeed = 90.0f;
	
	//Create Pawn sensing component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensing->SensingInterval = .5f; //  times per second
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->SetPeripheralVisionAngle(85.f);
}

// This actors basic collisions
void AAI_Bot_Character::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Arrow")) {

		Power -= 26.0f;

		if (Power > 100.0f)
			Power = 100.0f;

		OtherActor->Destroy();
	}

	if (OtherActor->ActorHasTag("OurSword"))
	{
		Power -= 104.0f;

		if (Power > 100.0f)
			Power = 100.0f;
	}
}

// This actors head collision
void AAI_Bot_Character::OnHeadCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag("Arrow")) {
		
		OtherActor->Destroy();
	}

	// If this actor has a collison with our sword. Then destory this actor and its sword.
	if (OtherActor->ActorHasTag("OurSword")) 
	{
		Destroy();

		EnemySword->Destroy();
		
		// try and spawn this body part
		if (BodyPart1class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{   // Get rotation
					const FRotator SpawnRotation = GetControlRotation();

					// Get location
					const FVector SpawnLocation = ((head != nullptr) ? head->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(headOffset);
					
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					
					// spawn body part at the spawn point
					World->SpawnActor<AAI_Bot_Character_Head>(BodyPart1class, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}
		
		// try and spawn this body part
		if (BodyPart2class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{   // Get rotation
					const FRotator SpawnRotation = GetControlRotation();

					// Get location
					const FVector SpawnLocation = ((head != nullptr) ? head->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(headOffset);
					
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					
					// spawn body part at the spawn point
					World->SpawnActor<AAI_Bot_Character_HeadlessBody>(BodyPart2class, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}
	}
}

//This actors right Body Collision
void AAI_Bot_Character::OnRightBodyCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Arrow")) {

		OtherActor->Destroy();
	}

	// If this actor has a collison with our sword. Then destory this actor and its sword.
	if (OtherActor->ActorHasTag("OurSword"))
	{
		Destroy();

		EnemySword->Destroy();
		
		// try and spawn this body part
		if (BodyPart3class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{
					// Get rotation
					const FRotator SpawnRotation = GetControlRotation();

					// Get location
					const FVector SpawnLocation = ((head != nullptr) ? head->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(headOffset);
					
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					
					// spawn body part at the spawn point
					World->SpawnActor<AAI_Bot_Character_RightUpBody>(BodyPart3class, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}
		
		// try and spawn this body part
		if (BodyPart4class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{   // Get rotation
					const FRotator SpawnRotation = GetControlRotation();

					// Get location
					const FVector SpawnLocation = ((head != nullptr) ? head->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(headOffset);
					
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					
					// spawn body part at the spawn point
					World->SpawnActor<AAI_Bot_Character_RightLowBody>(BodyPart4class, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}
	}
}

//This actors left body collision
void AAI_Bot_Character::OnLeftBodyCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Arrow")) {

		OtherActor->Destroy();
	}
	
	// If this actor has a collison with our sword. Then destory this actor and its sword.
	if (OtherActor->ActorHasTag("OurSword"))
	{
		Destroy();

		EnemySword->Destroy();
		
		// try and spawn this body part
		if (BodyPart5class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{
					// Get rotation
					const FRotator SpawnRotation = GetControlRotation();

					// Get location
					const FVector SpawnLocation = ((head != nullptr) ? head->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(headOffset);
					
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					
					// try and spawn this body part
					World->SpawnActor<AAI_Bot_Character_LeftUpBody>(BodyPart5class, SpawnLocation, SpawnRotation, ActorSpawnParams);
				
				}
			}
		}
		
		// try and spawn this body part
		if (BodyPart6class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{
					// Get rotation
					const FRotator SpawnRotation = GetControlRotation();

					// Get location
					const FVector SpawnLocation = ((head != nullptr) ? head->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(headOffset);
					
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					
					// try and spawn this body part
					World->SpawnActor<AAI_Bot_Character_LeftLowBody>(BodyPart6class, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}
	}
}

// This actors right arm collision
void AAI_Bot_Character::OnRightArmCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Arrow")) {

		OtherActor->Destroy();
	}
	
	// If this actor has a collison with our sword. Then destory this actor and its sword.
	if (OtherActor->ActorHasTag("OurSword"))
	{ 
		Destroy();

		EnemySword->Destroy();
		
		// try and spawn this body part
		if (BodyPart7class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{   // Get rotation
					const FRotator SpawnRotation = GetControlRotation();

					// Get location
					const FVector SpawnLocation = ((head != nullptr) ? head->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(headOffset);
					
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					
					// try and spawn this body part
					World->SpawnActor<AAI_Bot_Character_NoLeftArmBody>(BodyPart7class, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}
		
		// try and spawn this body part
		if (BodyPart8class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{
					// Get rotaion
					const FRotator SpawnRotation = GetControlRotation();

					// Get location
					const FVector SpawnLocation = ((head != nullptr) ? head->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(headOffset);
					
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					
					// try and spawn this body part
					World->SpawnActor<AAI_Bot_Character_LeftArm>(BodyPart8class, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}
	}
}

//This actors left arm collision
void AAI_Bot_Character::OnLeftArmCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Arrow")) {

		OtherActor->Destroy();
	}
	
	// If this actor has a collison with our sword. Then destory this actor and its sword.
	if (OtherActor->ActorHasTag("OurSword"))
	{
		Destroy();

		EnemySword->Destroy();
		
		// try and spawn this body part
		if (BodyPart9class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{
					// Get rotation
					const FRotator SpawnRotation = GetControlRotation();

					// Get location
					const FVector SpawnLocation = ((head != nullptr) ? head->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(headOffset);
					
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					
					// try and spawn this body part
					World->SpawnActor<AAI_Bot_Character_NoRightArmBody>(BodyPart9class, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}
		
		// try and spawn this body part
		if (BodyPart10class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{
					// Get rotation
					const FRotator SpawnRotation = GetControlRotation();

					// Get location
					const FVector SpawnLocation = ((head != nullptr) ? head->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(headOffset);
					
					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					
					// try and spawn this body part
					World->SpawnActor<AAI_Bot_Character_RightArm>(BodyPart10class, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AAI_Bot_Character::BeginPlay()
{
	Super::BeginPlay();
	
	PawnSensing->OnSeePawn.AddDynamic(this, &AAI_Bot_Character::SeePawn);

	// Spawn this actors weapon at the start of the game
	FActorSpawnParameters SpawnParems;
	SpawnParems.bNoFail = true;
	SpawnParems.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Attach the Enemy sword to the chosen bone
	if (Enemy1class)
	{
		FTransform EnemySword1Transform;

		EnemySword1Transform.SetLocation(FVector::ZeroVector);
		EnemySword1Transform.SetRotation(FQuat(FRotator::ZeroRotator));


		EnemySword = GetWorld()->SpawnActor<AEnemySword>(Enemy1class, EnemySword1Transform, SpawnParems);
		if (EnemySword)
		{

			EnemySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Thigh_Sword_Enemy"));
		}
	}

	// Get this actors capsule
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Character::OnBeginOverLap);
}

// Pawn sensing component used for the sword attack
void AAI_Bot_Character::SeePawn(APawn* Pawn)
{
	// If this enemy character detects our character. Then change walk speed and do the sword attack
	if (PawnSensing->OnSeePawn, true) {

		AMyCharacter* Actor = Cast<AMyCharacter>(PawnSensing);

		GetCharacterMovement()->MaxWalkSpeed = 400.0f;

		// Attach the sword to this characters hand
		EnemySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Sword_Hand_R_Enemy"));
		
		// Play the sword attack animation
		GetMesh()->SetAnimation(Sword_Attack_1);
		GetMesh()->GetAnimInstance()->Montage_Play(Sword_Attack_1);

		if (Sword_Attack_1)
		{
			// try and play the sound if specified
			if (SwordAttackSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, SwordAttackSound, GetActorLocation());
			}
		}
	}
}

// Called every frame
void AAI_Bot_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if this actors health is lower then 3. stimulate physics and destroy all this actors collisions.
	if (Power < 3) {

		if (!bDead) {

			EnemySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Thigh_Sword_Enemy"));
			bDead = true;

			GetMesh()->SetSimulatePhysics(true);
			
			Controller->Destroy();
			
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

// Called to bind functionality to input
void AAI_Bot_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// This enemies team ID
FGenericTeamId AAI_Bot_Character::GetGenericTeamId() const
{
	return FGenericTeamId();
}

// Sword attack
void AAI_Bot_Character::SwordAttack()
{
	// Play sword attack
	GetMesh()->SetAnimation(Sword_Attack_1);
	GetMesh()->GetAnimInstance()->Montage_Play(Sword_Attack_1);
}
		
// Equipped sword animation	
void AAI_Bot_Character::EquippedSword()
{
	// Play equipped sword animation
	if (Equipped_Sword)
	{  
	   GetMesh()->SetAnimation(Equipped_Sword);
	   GetMesh()->PlayAnimation(Equipped_Sword, false);
	}
}

// unequipped sword animation
void AAI_Bot_Character::UnequippedSword()
{
	if (Unequipped_Sword)
	{
		// Play unequipped sword animation
		GetMesh()->SetAnimation(Unequipped_Sword);
		GetMesh()->PlayAnimation(Unequipped_Sword, false);
	}
}

// Set this actors view point
void AAI_Bot_Character::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation("head");
	
	Rotation = GetActorRotation();
	Rotation.Yaw -= GetMesh()->GetSocketTransform("head", RTS_ParentBoneSpace).Rotator().Roll;
}
