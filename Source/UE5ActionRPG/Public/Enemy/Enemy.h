// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class AAIController;
class UHealthBarComponent;
class UAttributeComponent;

UCLASS()
class UE5ACTIONRPG_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	double CalculateImpactAngle(const FVector& ImpactPoint) const;
	void GetHitReactMontageSection(double ImpactAngle, FName& Section);
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;
	UFUNCTION()
	void OnSeePawnHandler(APawn* Pawn);

protected:
	virtual void BeginPlay() override;
	void PlayMontage(const FName& SectionName, UAnimMontage* Montage) const;
	void Die();
	bool InTargetRage(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

private:
	// Gameplay Variables
	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;

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

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();
	void CheckCombatTarget();
	void CheckPatrolTarget();

	UPROPERTY()
	TObjectPtr<AAIController> EnemyController;
	
	UPROPERTY(EditInstanceOnly, Category="AI Stuff", BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI Stuff")
	TArray<TObjectPtr<AActor>> PatrolTargets;
	
	// Components
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> AttributeComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> SensingComponent;
	
	// VFX
	UPROPERTY(EditAnywhere, Category="Visual Effects")
	TObjectPtr<UParticleSystem> HitParticles;

	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> DeathMontage;

	// Sounds
	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<USoundBase> HitSound;



};
