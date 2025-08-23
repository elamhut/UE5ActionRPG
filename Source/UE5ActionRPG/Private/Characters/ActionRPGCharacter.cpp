// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionRPGCharacter.h"

#include "GroomComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/ActionRPGHUD.h"
#include "HUD/ActionRPGOverlay.h"
#include "Item/Soul.h"
#include "Item/Treasure.h"
#include "Item/Weapon.h"


// Sets default values
AActionRPGCharacter::AActionRPGCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 800.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");


	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AActionRPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngageableTarget"));
	
	InitializeHUDOverlay();
}

void AActionRPGCharacter::Die()
{
	Super::Die();

	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

// Called every frame
void AActionRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttributeComponent && ActionRPGOverlay)
	{
		AttributeComponent->RegenerateStamina(DeltaTime);
		ActionRPGOverlay->SetStaminaBarPercent(AttributeComponent->GetStaminaPercent());
	}
}

void AActionRPGCharacter::DoMove(const FVector2D& Vector)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
	const FVector XDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector YDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(XDirection * Vector.X);
	AddMovementInput(YDirection * Vector.Y);
}


void AActionRPGCharacter::DoLook(const FVector2D& Vector)
{
	AddControllerYawInput(Vector.X);
	AddControllerPitchInput(Vector.Y);
}

bool AActionRPGCharacter::IsUnoccupied() { return ActionState == EActionState::EAS_Unoccupied; }

void AActionRPGCharacter::DoJump()
{
	if (IsUnoccupied())
		Jump();
}

void AActionRPGCharacter::PlayEquipMontage(const FName SectionName) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

bool AActionRPGCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

void AActionRPGCharacter::DoAttack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &AActionRPGCharacter::PlayerAttackEnd);
	}
}

void AActionRPGCharacter::PlayerAttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	ActionState = EActionState::EAS_Unoccupied; 
}

void AActionRPGCharacter::PlayerDodgeEnd(UAnimMontage* Montage, bool bInterrupted)
{
	ActionState = EActionState::EAS_Unoccupied; 
}

void AActionRPGCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AActionRPGCharacter::AttachWeaponToBack() const
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AActionRPGCharacter::AttachWeaponToHand() const
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AActionRPGCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AActionRPGCharacter::CanDisarm() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AActionRPGCharacter::CanArm() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void AActionRPGCharacter::EquipWeapon(AWeapon* Weapon) {
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AActionRPGCharacter::Disarm() {
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AActionRPGCharacter::Arm() {
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AActionRPGCharacter::DoEquip()
{
	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

bool AActionRPGCharacter::IsOccupied() { return ActionState != EActionState::EAS_Unoccupied; }

bool AActionRPGCharacter::HasEnoughStamina() { return AttributeComponent->GetStamina() > AttributeComponent->GetDodgeCost(); }

void AActionRPGCharacter::DoDodge()
{
	if (IsOccupied() || !HasEnoughStamina()) return;
		
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodging;
	AttributeComponent->UseStamina(AttributeComponent->GetDodgeCost());
	ActionRPGOverlay->SetStaminaBarPercent(AttributeComponent->GetStaminaPercent());
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &AActionRPGCharacter::PlayerDodgeEnd);
}

void AActionRPGCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	
	SetWeaponCollisionType(ECollisionEnabled::NoCollision);
	if (AttributeComponent && AttributeComponent->GetCurrentHealthPercent() > 0.f)
		ActionState = EActionState::EAS_HitReaction;
}

void AActionRPGCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AActionRPGCharacter::AddSouls(ASoul* Soul)
{
	if (AttributeComponent && ActionRPGOverlay)
	{
		AttributeComponent->AddSouls(Soul->GetSouls());
		ActionRPGOverlay->SetSouls(AttributeComponent->GetSouls());
	}
}

void AActionRPGCharacter::AddGold(ATreasure* Gold)
{
	if (AttributeComponent && ActionRPGOverlay)
	{
		AttributeComponent->AddSouls(Gold->GetGold());
		ActionRPGOverlay->SetGold(AttributeComponent->GetGold());
	}
}


float AActionRPGCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(Damage);
	UpdateHealthBar();
	
	return Damage;
}

void AActionRPGCharacter::InitializeHUDOverlay()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AActionRPGHUD* ActionRPGHUD = Cast<AActionRPGHUD>(PlayerController->GetHUD()))
		{
			if (ActionRPGOverlay = ActionRPGHUD->GetOverlay())
			{
				ActionRPGOverlay->SetHealthBarPercent(AttributeComponent->GetCurrentHealthPercent());
				ActionRPGOverlay->SetStaminaBarPercent(AttributeComponent->GetCurrentHealthPercent());
				ActionRPGOverlay->SetGold(0);
				ActionRPGOverlay->SetSouls(0);
			}
		}
	}
}

void AActionRPGCharacter::UpdateHealthBar()
{
	if (ActionRPGOverlay && AttributeComponent)
		ActionRPGOverlay->SetHealthBarPercent(AttributeComponent->GetCurrentHealthPercent());
}
