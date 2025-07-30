// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActionRPGAnimInstance.generated.h"

class UCharacterMovementComponent;
class AActionRPGCharacter;
/**
 * 
 */
UCLASS()
class UE5ACTIONRPG_API UActionRPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActionRPGCharacter> ActionRPGCharacter;

	UPROPERTY(BlueprintReadWrite, Category="Movement")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(BlueprintReadWrite, Category="Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadWrite)
	bool bIsFalling;
};
