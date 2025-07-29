// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "ActionRPGPlayerController.generated.h"

class AActionRPGCharacter;
class ABird;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class UE5ACTIONRPG_API AActionRPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AActionRPGPlayerController();

protected:
	virtual void BeginPlay() override;
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJump();
	void HandleAttack();
	void HandleEquip();
	void HandleDodge();

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<AActionRPGCharacter> PlayerChar;
	
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;

	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputLocalPlayerSubsystem;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input Properties")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input Properties | Actions")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input Properties | Actions")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input Properties | Actions")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input Properties | Actions")
	TObjectPtr<UInputAction> AttackAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input Properties | Actions")
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input Properties | Actions")
	TObjectPtr<UInputAction> DodgeAction;
};
