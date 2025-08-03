// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionRPGCharacter.h"

#include "GroomComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
}

// Called every frame
void AActionRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AActionRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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

void AActionRPGCharacter::DoJump()
{
	Jump();
}

void AActionRPGCharacter::PlayAttackMontage() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
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

bool AActionRPGCharacter::CanAttack() const
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
		AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &AActionRPGCharacter::AttackEnded);
		UE_LOG(LogTemp, Warning, TEXT("Current Action State %ls"), *UEnum::GetValueAsString(ActionState));
	}
}

void AActionRPGCharacter::AttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ActionState = EActionState::EAS_Unoccupied;
	UE_LOG(LogTemp, Warning, TEXT("Current Action State %ls"), *UEnum::GetValueAsString(ActionState));
}

void AActionRPGCharacter::Disarm() const
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AActionRPGCharacter::Arm() const
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

void AActionRPGCharacter::SetWeaponCollisionType(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponCollider())
		EquippedWeapon->GetWeaponCollider()->SetCollisionEnabled(CollisionEnabled);
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

void AActionRPGCharacter::DoEquip()
{
	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void AActionRPGCharacter::DoDodge()
{
	UE_LOG(LogTemp, Warning, TEXT("DODGING!"));
}
