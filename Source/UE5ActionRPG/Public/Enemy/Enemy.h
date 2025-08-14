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
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual void Destroyed() override;

	virtual float TakeDamage(float Damage,
							 const FDamageEvent& DamageEvent,
							 AController* EventInstigator,
							 AActor* DamageCauser) override;
	
	UFUNCTION()
	void OnSeePawnHandler(APawn* Pawn);

protected:

	
	virtual void BeginPlay() override;
	virtual void Die() override;
	virtual void DoAttack() override;
	virtual void PlayAttackMontage() override;
	void MoveToTarget(AActor* Target);
	bool InTargetRage(AActor* Target, double Radius);
	AActor* ChoosePatrolTarget();

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose;
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
private:
	// Gameplay Variables
	UPROPERTY(EditAnywhere)
	double CombatRadius{500.f};
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

	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;
	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;
	
	void PatrolTimerFinished();
	void LoseInterest();
	void StartPatrolState();
	bool IsOutsideCombatRadius();
	void ChaseCombatTarget();
	bool IsAttacking();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsOutsideAttackRadius();
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void StartAttackTimer();

	UPROPERTY()
	TObjectPtr<AAIController> EnemyController;

	UPROPERTY(EditInstanceOnly, Category="AI Stuff", BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI Stuff")
	TArray<TObjectPtr<AActor>> PatrolTargets;

	// Components
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> SensingComponent;
};
