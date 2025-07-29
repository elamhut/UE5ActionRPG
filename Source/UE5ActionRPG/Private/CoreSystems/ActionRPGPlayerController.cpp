// Fill out your copyright notice in the Description page of Project Settings.


#include "UE5ActionRPG/Public/CoreSystems/ActionRPGPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/ActionRPGCharacter.h"

AActionRPGPlayerController::AActionRPGPlayerController() {}

void AActionRPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerChar = CastChecked<AActionRPGCharacter>(GetPawn());

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	checkf(LocalPlayer, TEXT("No LocalPlayer Found..."))

	EnhancedInputLocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	checkf(EnhancedInputLocalPlayerSubsystem, TEXT("No EnhancedInputLocalPlayerSubsystem Found..."))
	checkf(InputMappingContext, TEXT("You forgot to set the InputMappingContext..."))

	EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
	                                   &AActionRPGPlayerController::HandleMove);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
	                                   &AActionRPGPlayerController::HandleLook);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
	                                   &AActionRPGPlayerController::HandleJump);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this,
	                                   &AActionRPGPlayerController::HandleAttack);
	EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this,
	                                   &AActionRPGPlayerController::HandleEquip);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this,
	                                   &AActionRPGPlayerController::HandleDodge);
}

void AActionRPGPlayerController::HandleMove(const FInputActionValue& Value)
{
	const FVector2D MoveVec = Value.Get<FVector2D>();
	PlayerChar->DoMove(MoveVec);
}

void AActionRPGPlayerController::HandleLook(const FInputActionValue& Value)
{
	const FVector2D LookVec = Value.Get<FVector2D>();
	PlayerChar->DoLook(LookVec);
}

void AActionRPGPlayerController::HandleJump()
{
	PlayerChar->DoJump();
}

void AActionRPGPlayerController::HandleAttack()
{
	PlayerChar->DoAttack();
}

void AActionRPGPlayerController::HandleEquip()
{
	PlayerChar->DoEquip();
}

void AActionRPGPlayerController::HandleDodge()
{
	PlayerChar->DoDodge();
}
