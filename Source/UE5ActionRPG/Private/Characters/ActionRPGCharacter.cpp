// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionRPGCharacter.h"


// Sets default values
AActionRPGCharacter::AActionRPGCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	AddMovementInput(GetActorForwardVector() * Vector.X);
	AddMovementInput(GetActorRightVector() * Vector.Y);
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
