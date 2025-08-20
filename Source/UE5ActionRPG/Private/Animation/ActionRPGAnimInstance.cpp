// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ActionRPGAnimInstance.h"

#include "Characters/ActionRPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UActionRPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (ActionRPGCharacter = Cast<AActionRPGCharacter>(TryGetPawnOwner()))
		MovementComponent = ActionRPGCharacter->GetCharacterMovement();
}

void UActionRPGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!MovementComponent) return;
	
	GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
	bIsFalling = MovementComponent->IsFalling();
	CharacterState = ActionRPGCharacter->GetCharacterState();
	ActionState = ActionRPGCharacter->GetActionState();
	DeathPose = ActionRPGCharacter->GetDeathPose();
}
