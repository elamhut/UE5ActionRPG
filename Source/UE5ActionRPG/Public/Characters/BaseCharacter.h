// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class UAttributeComponent;
class AWeapon;

UCLASS()
class UE5ACTIONRPG_API ABaseCharacter : public ACharacter, public IHitInterface
{
    GENERATED_BODY()

public:
    ABaseCharacter();

protected:
    virtual void BeginPlay() override;

    virtual bool CanAttack();
    bool IsAlive();

    virtual void DoAttack();
    virtual void Die();
    virtual void HandleDamage(float DamageAmount);
    double CalculateImpactAngle(const FVector& ImpactPoint) const;
    FName GetHitReactMontageSection(double ImpactAngle);
    virtual int32 PlayAttackMontage();
    virtual int32 PlayDeathMontage();
    void PlayMontageSection(const FName& SectionName, UAnimMontage* Montage) const;
    int32 PlayRandomMontageSection(const TArray<FName>& SectionNames, UAnimMontage* Montage) const;

    UFUNCTION(BlueprintCallable)
    virtual void AttackEnd();

    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisionType(ECollisionEnabled::Type CollisionEnabled);

    void PlayHitSound(const FVector& ImpactPoint);
    void SpawnHitParticles(const FVector& ImpactPoint);

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAttributeComponent> AttributeComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<AWeapon> EquippedWeapon;

    UPROPERTY(EditAnywhere, Category="Montages")
    TArray<FName> AttackMontageSections;
    UPROPERTY(EditDefaultsOnly, Category="Montages")
    TObjectPtr<UAnimMontage> AttackMontage;

    UPROPERTY(EditAnywhere, Category="Montages")
    TArray<FName> HitReactMontageSections;
    UPROPERTY(EditDefaultsOnly, Category="Montages")
    TObjectPtr<UAnimMontage> HitReactMontage;

    UPROPERTY(EditAnywhere, Category="Montages")
    TArray<FName> DeathMontageSections;
    UPROPERTY(EditDefaultsOnly, Category="Montages")
    TObjectPtr<UAnimMontage> DeathMontage;

private:
    // VFX
    UPROPERTY(EditAnywhere, Category="Visual Effects")
    TObjectPtr<UParticleSystem> HitParticles;

    // Sounds
    UPROPERTY(EditAnywhere, Category="Sounds")
    TObjectPtr<USoundBase> HitSound;
};
