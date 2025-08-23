// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
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

    FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

protected:
    virtual void BeginPlay() override;

    virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
    
    virtual bool CanAttack();
    bool IsAlive();

    virtual void DoAttack();
    virtual void Die();
    virtual void HandleDamage(float DamageAmount);
    double CalculateImpactAngle(const FVector& ImpactPoint) const;
    FName GetHitReactMontageSection(double ImpactAngle);
    virtual int32 PlayAttackMontage();
    virtual int32 PlayDeathMontage();
    virtual void PlayDodgeMontage();
    void PlayMontageSection(const FName& SectionName, UAnimMontage* Montage) const;
    int32 PlayRandomMontageSection(const TArray<FName>& SectionNames, UAnimMontage* Montage) const;
    void StopAttackMontage();
    void DisableMeshCollision();

    UFUNCTION(BlueprintCallable)
    FVector GetTranslationWarpTarget();
    
    UFUNCTION(BlueprintCallable)
    FVector GetRotationWarpTarget();
    
    UFUNCTION(BlueprintCallable)
    virtual void AttackEnd();

    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisionType(ECollisionEnabled::Type CollisionEnabled);

    void PlayHitSound(const FVector& ImpactPoint);
    void SpawnHitParticles(const FVector& ImpactPoint);

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<AActor> CombatTarget;

    UPROPERTY(EditAnywhere)
    float WarpTargetDistance{75.f};

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
    UPROPERTY(BlueprintReadOnly)
    TEnumAsByte<EDeathPose> DeathPose;

    UPROPERTY(EditDefaultsOnly, Category="Montages")
    TObjectPtr<UAnimMontage> DodgeMontage;
    
private:
    // VFX
    UPROPERTY(EditAnywhere, Category="Visual Effects")
    TObjectPtr<UParticleSystem> HitParticles;

    // Sounds
    UPROPERTY(EditAnywhere, Category="Sounds")
    TObjectPtr<USoundBase> HitSound;
};
