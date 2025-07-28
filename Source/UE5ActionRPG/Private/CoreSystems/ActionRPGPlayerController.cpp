// Fill out your copyright notice in the Description page of Project Settings.


#include "UE5ActionRPG/Public/CoreSystems/ActionRPGPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UE5ActionRPG/Public/Pawns/Bird.h"

AActionRPGPlayerController::AActionRPGPlayerController()
{
}

void AActionRPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Bird = Cast<ABird>(GetPawn());
	checkf(Bird, TEXT("No BirdPawn Found..."))
	
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	checkf(LocalPlayer, TEXT("No LocalPlayer Found..."))
	
	EnhancedInputLocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	checkf(EnhancedInputLocalPlayerSubsystem, TEXT("No EnhancedInputLocalPlayerSubsystem Found..."))
	
	checkf(InputMappingContext, TEXT("You forgot to set the InputMappingContext..."))

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("No EnhancedInputComponent Found..."))
	
	EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AActionRPGPlayerController::HandleMove);
}

void AActionRPGPlayerController::HandleMove(const FInputActionValue& Value)
{
	const FVector2D MoveVec = Value.Get<FVector2D>();
	Bird->DoMove(MoveVec);
}

