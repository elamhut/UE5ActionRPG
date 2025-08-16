// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "Characters/BaseCharacter.h"
#include "Enemy.generated.h"

class AAIController;
class UPawnSensingComponent;
class UHealthBarComponent;
class UAttributeComponent;

UCLASS()
class UE5ACTIONRPG_API AEnemy : public ABaseCharacter
{
    GENERATED_BODY()

public:
    // Unreal
    AEnemy();
    virtual void Tick(float DeltaTime) override;
    virtual void Destroyed() override;

    // IHitInterface
    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

    // AActor
    virtual float TakeDamage(float Damage,
                             const FDamageEvent& DamageEvent,
                             AController* EventInstigator,
                             AActor* DamageCauser) override;

protected:
    UPROPERTY(BlueprintReadOnly)
    TEnumAsByte<EDeathPose> DeathPose;
    UPROPERTY(BlueprintReadOnly)
    EEnemyState EnemyState = EEnemyState::EES_Patrolling;

    virtual void BeginPlay() override;
    virtual void Die() override;
    virtual void DoAttack() override;
    virtual void HandleDamage(float DamageAmount) override;
    virtual void AttackEnd() override;
    virtual bool CanAttack() override;

    virtual int32 PlayDeathMontage() override;

private:
    // Gameplay Variables
    UPROPERTY(EditAnywhere)
    double CombatRadius{1000.f};
    UPROPERTY(EditAnywhere)
    double AttackRadius{150.f};
    UPROPERTY(EditAnywhere)
    double PatrolRadius{200.f};
    UPROPERTY(EditAnywhere)
    float PatrolWaitMin{2.f};
    UPROPERTY(EditAnywhere)
    float PatrolWaitMax{5.f};
    UPROPERTY(EditAnywhere)
    float PatrolSpeed{150.f};
    UPROPERTY(EditAnywhere)
    float ChasingSpeed{400.f};
    UPROPERTY(EditAnywhere)
    float AttackMinTimer{0.5f};
    UPROPERTY(EditAnywhere)
    float AttackMaxTimer{1.5f};
    UPROPERTY(EditAnywhere)
    float DeathLifespan{15.f};

    FTimerHandle PatrolTimer;
    FTimerHandle AttackTimer;

    UPROPERTY()
    TObjectPtr<AAIController> EnemyController;

    UPROPERTY(EditInstanceOnly, Category="AI Stuff", BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
    TObjectPtr<AActor> PatrolTarget;
    UPROPERTY(EditInstanceOnly, Category="AI Stuff")
    TArray<TObjectPtr<AActor>> PatrolTargets;
    UPROPERTY()
    TObjectPtr<AActor> CombatTarget;

    // Components
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UHealthBarComponent> HealthBarWidget;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UPawnSensingComponent> SensingComponent;
    UPROPERTY(EditAnywhere)
    TSubclassOf<AWeapon> WeaponClass;

    void CheckPatrolTarget();
    void CheckCombatTarget();
    void PatrolTimerFinished();
    void LoseInterest();
    void StartPatrolState();
    void ChaseCombatTarget();
    bool IsOutsideCombatRadius();
    bool IsOutsideAttackRadius();
    bool IsInsideAttackRadius();
    bool IsChasing();
    bool IsAttacking();
    bool IsDead();
    bool IsEngaged();
    void StartAttackTimer();
    void ClearAttackTimer();
    void ClearPatrolTimer();
    void DisableCollision();
    void MoveToTarget(AActor* Target);
    bool InTargetRage(AActor* Target, double Radius);
    AActor* ChoosePatrolTarget();

    UFUNCTION()
    void OnSeePawnHandler(APawn* Pawn);
};
