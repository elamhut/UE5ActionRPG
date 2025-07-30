// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionRPGCharacter.h"

#include "GroomComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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

void AActionRPGCharacter::DoAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("ATTACKING!"));
}

void AActionRPGCharacter::DoEquip()
{
	UE_LOG(LogTemp, Warning, TEXT("EQUIPPING!"));
}

void AActionRPGCharacter::DoDodge()
{
	UE_LOG(LogTemp, Warning, TEXT("DODGING!"));
}
