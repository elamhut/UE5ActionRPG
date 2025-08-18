// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "HUD/HealthBarComponent.h"
#include "CharacterTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AITypes.h"
#include "Characters/ActionRPGCharacter.h"
#include "Item/Weapon.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemy::AEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    USkeletalMeshComponent* SkeletalMesh{GetMesh()};
    SkeletalMesh->SetCollisionObjectType(ECC_WorldDynamic);
    SkeletalMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    SkeletalMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    SkeletalMesh->SetGenerateOverlapEvents(true);

    HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComponent"));
    HealthBarWidget->SetupAttachment(GetRootComponent());

    SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    SensingComponent->SetPeripheralVisionAngle(45.f);
    SensingComponent->SightRadius = 3000.f;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    HealthBarWidget->SetVisibility(false);

    EnemyController = Cast<AAIController>(GetController());
    MoveToTarget(PatrolTarget);

    SensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnSeePawnHandler);

    AWeapon* DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
    DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
    EquippedWeapon = DefaultWeapon;

    Tags.Add(FName("Enemy"));
}

void AEnemy::DisableCollision() { GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); }

void AEnemy::Die()
{
    EnemyState = EEnemyState::EES_Dead;
    PlayDeathMontage();
    ClearAttackTimer();
    HealthBarWidget->SetVisibility(false);
    DisableCollision();
    SetLifeSpan(DeathLifespan);
    SetWeaponCollisionType(ECollisionEnabled::NoCollision);
}

void AEnemy::DoAttack()
{
    EnemyState = EEnemyState::EES_Engaged;
    Super::DoAttack();
    PlayAttackMontage();
}

void AEnemy::HandleDamage(float DamageAmount)
{
    Super::HandleDamage(DamageAmount);
    HealthBarWidget->SetHealthPercent(AttributeComponent->GetCurrentHealthPercent());
}

void AEnemy::AttackEnd()
{
    Super::AttackEnd();
    EnemyState = EEnemyState::EES_NoState;
    CheckCombatTarget();
}

bool AEnemy::InTargetRage(AActor* Target, double Radius)
{
    if (!Target) return false;

    const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

    return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
    if (!EnemyController || !Target)
    {
        return;
    }

    FAIMoveRequest MoveReq;
    MoveReq.SetGoalActor(Target);
    MoveReq.SetAcceptanceRadius(20.f);
    EnemyController->MoveTo(MoveReq);
}

AActor* AEnemy::ChoosePatrolTarget()
{
    TArray<AActor*> ValidTargets;
    for (AActor* Target : PatrolTargets)
    {
        if (Target != PatrolTarget)
        {
            ValidTargets.AddUnique(Target);
        }
    }

    const int32 RandomTarget = FMath::RandRange(0, ValidTargets.Num() - 1);
    if (PatrolTarget = ValidTargets[RandomTarget])
        return PatrolTarget;

    return nullptr;
}

void AEnemy::PatrolTimerFinished()
{
    MoveToTarget(PatrolTarget);
}

void AEnemy::LoseInterest()
{
    CombatTarget = nullptr;
    HealthBarWidget->SetVisibility(false);
}

void AEnemy::StartPatrolState()
{
    EnemyState = EEnemyState::EES_Patrolling;
    GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
    MoveToTarget(PatrolTarget);
}

bool AEnemy::IsOutsideCombatRadius() { return !InTargetRage(CombatTarget, CombatRadius); }
bool AEnemy::IsOutsideAttackRadius() { return !InTargetRage(CombatTarget, AttackRadius); }
bool AEnemy::IsInsideAttackRadius()  { return InTargetRage(CombatTarget, AttackRadius); }
bool AEnemy::IsChasing()             { return EnemyState == EEnemyState::EES_Chasing; }
bool AEnemy::IsAttacking()           { return EnemyState == EEnemyState::EES_Attacking; }

void AEnemy::ChaseCombatTarget()
{
    EnemyState = EEnemyState::EES_Chasing;
    GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
    MoveToTarget(CombatTarget);
}

bool AEnemy::IsEngaged() { return EnemyState == EEnemyState::EES_Engaged; }

void AEnemy::CheckCombatTarget()
{
    if (IsOutsideCombatRadius())
    {
        ClearAttackTimer();
        LoseInterest();

        if (!IsEngaged())
            StartPatrolState();
    }
    else if (IsOutsideAttackRadius() && !IsChasing())
    {
        ClearAttackTimer();

        if (!IsEngaged())
            ChaseCombatTarget();
    }
    else if (CanAttack())
    {
        StartAttackTimer();
    }
}

void AEnemy::CheckPatrolTarget()
{
    if (InTargetRage(PatrolTarget, PatrolRadius))
    {
        PatrolTarget = ChoosePatrolTarget();
        const float RandWait = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
        GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, RandWait);
    }
}

void AEnemy::StartAttackTimer()
{
    EnemyState = EEnemyState::EES_Attacking;
    const float AttackTime = FMath::RandRange(AttackMinTimer, AttackMaxTimer);
    GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::DoAttack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
    GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::IsDead() { return EnemyState == EEnemyState::EES_Dead; }

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsDead()) return;

    if (EnemyState > EEnemyState::EES_Patrolling)
    {
        CheckCombatTarget();
    }
    else
        CheckPatrolTarget();
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
    Super::GetHit_Implementation(ImpactPoint, Hitter);
    
    if (!IsDead())
        HealthBarWidget->SetVisibility(true);
    
    ClearPatrolTimer();
    ClearAttackTimer();
    SetWeaponCollisionType(ECollisionEnabled::NoCollision);

    StopAttackMontage();
}

float AEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
    HandleDamage(Damage);
    CombatTarget = EventInstigator->GetPawn();

    if (IsInsideAttackRadius())
    {
        EnemyState = EEnemyState::EES_Attacking;
    }
    else if (IsOutsideAttackRadius())
    {
        ChaseCombatTarget();
    }

    return Damage;
}

void AEnemy::Destroyed()
{
    Super::Destroyed();

    if (EquippedWeapon)
        EquippedWeapon->Destroy();
}

void AEnemy::ClearPatrolTimer() { GetWorldTimerManager().ClearTimer(PatrolTimer); }

void AEnemy::OnSeePawnHandler(APawn* Pawn)
{
    const bool bShouldChaseTarget = EnemyState != EEnemyState::EES_Dead &&
                                    EnemyState != EEnemyState::EES_Chasing &&
                                    EnemyState < EEnemyState::EES_Attacking &&
                                    Pawn->ActorHasTag(FName("EngageableTarget"));

    if (bShouldChaseTarget)
    {
        CombatTarget = Pawn;
        ClearPatrolTimer();
        ChaseCombatTarget();
    }
}

bool AEnemy::CanAttack()
{
    const bool bCanAttack = IsInsideAttackRadius() &&
                            !IsAttacking() &&
                            !IsEngaged() &&
                            !IsDead();

    return bCanAttack;
}

int32 AEnemy::PlayDeathMontage()
{
    const int32 Selection = Super::PlayDeathMontage();
    TEnumAsByte<EDeathPose> Pose(Selection);

    if (Pose < EDeathPose::EDP_MAX)
        DeathPose = Pose;

    return Selection;
}
