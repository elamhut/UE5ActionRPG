// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "ActionRPGCharacter.generated.h"

class UActionRPGOverlay;
class AWeapon;
class AItem;
class UGroomComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UE5ACTIONRPG_API AActionRPGCharacter : public ABaseCharacter
{
    GENERATED_BODY()

public:
    AActionRPGCharacter();
    virtual void Tick(float DeltaTime) override;
    void DoMove(const FVector2D& Vector);
    void DoLook(const FVector2D& Vector);
    bool IsUnoccupied();
    void DoJump();
    virtual void DoAttack() override;
    void DoEquip();
    void DoDodge();

    // IHitInterface
    virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

    // AActor
    virtual float TakeDamage(float Damage,
                             const FDamageEvent& DamageEvent,
                             AController* EventInstigator,
                             AActor* DamageCauser) override;
    
    FORCEINLINE TObjectPtr<AItem> GetOverlappingItem() const { return OverlappingItem; }
    FORCEINLINE void SetOverlappingItem(const TObjectPtr<AItem>& OverlappedItem) { this->OverlappingItem = OverlappedItem; }
    FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
    FORCEINLINE EActionState GetActionState() const { return ActionState; }

protected:
    virtual void BeginPlay() override;
    virtual void Die() override;
    virtual bool CanAttack() override;
    bool CanDisarm() const;
    bool CanArm() const;

    UFUNCTION()
    void PlayerAttackEnd(UAnimMontage* Montage, bool bInterrupted);
    
    UFUNCTION(BlueprintCallable)
    void HitReactEnd();

    void PlayEquipMontage(const FName SectionName) const;

    void EquipWeapon(AWeapon* Weapon);
    void Disarm();
    void Arm();

    UFUNCTION(BlueprintCallable)
    void AttachWeaponToBack() const;

    UFUNCTION(BlueprintCallable)
    void AttachWeaponToHand() const;

    UFUNCTION(BlueprintCallable)
    void FinishEquipping();

private:
    ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

    UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
    EActionState ActionState = EActionState::EAS_Unoccupied;

    UPROPERTY(EditAnywhere)
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY()
    TObjectPtr<UActionRPGOverlay> ActionRPGOverlay;

    UPROPERTY(VisibleAnywhere, Category="Hair")
    TObjectPtr<UGroomComponent> Hair;

    UPROPERTY(VisibleAnywhere, Category="Hair")
    TObjectPtr<UGroomComponent> Eyebrows;

    UPROPERTY(VisibleInstanceOnly)
    TObjectPtr<AItem> OverlappingItem;

    // Animation Montages
    UPROPERTY(EditDefaultsOnly, Category="Montages")
    TObjectPtr<UAnimMontage> EquipMontage;

    void InitializeHUDOverlay();
    void UpdateHealthBar();
    
};
