// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "ActionRPGCharacter.generated.h"

class UGroomComponent;
class UCameraComponent;

UCLASS()
class UE5ACTIONRPG_API AActionRPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AActionRPGCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void DoMove(const FVector2D& Vector);
	virtual void DoLook(const FVector2D& Vector);
	virtual void DoJump();
	virtual void DoAttack();
	virtual void DoEquip();
	virtual void DoDodge();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Hair")
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(VisibleAnywhere, Category="Hair")
	TObjectPtr<UGroomComponent> Eyebrows;
};
