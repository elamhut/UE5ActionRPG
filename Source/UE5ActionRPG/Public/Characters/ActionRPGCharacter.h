// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "ActionRPGCharacter.generated.h"

class AItem;
class UGroomComponent;
class USpringArmComponent;
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

	[[nodiscard]] FORCEINLINE TObjectPtr<AItem> GetOverlappingItem() const { return OverlappingItem; }
	FORCEINLINE void SetOverlappingItem(const TObjectPtr<AItem>& OverlappedItem) { this->OverlappingItem = OverlappedItem; }
	[[nodiscard]] FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	

protected:
	virtual void BeginPlay() override;
	void PlayAttackMontage() const;
	bool CanAttack() const;

	UFUNCTION(BlueprintCallable)
	void AttackEnded(UAnimMontage* Montage, bool bInterrupted);

private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Hair")
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(VisibleAnywhere, Category="Hair")
	TObjectPtr<UGroomComponent> Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;

	// Animation Montages

	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> AttackMontage;

};
