// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "HUD/HealthBarComponent.h"
#include "CharacterTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"


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

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComponent"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

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

}

void AEnemy::PlayMontage(const FName& SectionName, UAnimMontage* Montage) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void AEnemy::Die()
{
	const int32 Selection = FMath::RandRange(0, 1);
	FName SectionName = FName();
	switch (Selection)
	{
	case 0:
		SectionName = FName("Death1");
		DeathPose = EDeathPose::EDP_Death1;
		break;
	case 1:
		SectionName = FName("Death2");
		DeathPose = EDeathPose::EDP_Death2;
		break;
	case 2:
		SectionName = FName("Death3");
		DeathPose = EDeathPose::EDP_Death3;
		break;
	default:
		break;
	}
	
	PlayMontage(SectionName, DeathMontage);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(20.f);
	HealthBarWidget->SetVisibility(false);
}

bool AEnemy::InTargetRage(AActor* Target, double Radius)
{
	if (!Target) return false;
	
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	DrawDebugSphere(GetWorld(), GetActorLocation(), 10.f, 12, FColor::Red, false);
	DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 10.f, 12, FColor::Red, false);
	
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (!EnemyController || !Target) return;
	
	FAIMoveRequest MoveReq;
	MoveReq.SetGoalActor(Target);
	MoveReq.SetAcceptanceRadius(15.f);
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

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CombatTarget)
	{
		if (!InTargetRage(CombatTarget, CombatRadius))
		{
			CombatTarget = nullptr;
			HealthBarWidget->SetVisibility(false);
		}
	}

	if (InTargetRage(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 5.f);
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

double AEnemy::CalculateImpactAngle(const FVector& ImpactPoint) const
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	/*	The equation to calculate the Cosine of Theta is:
	 *	Forward * ToHit = |Forward||ToHit| * cos(theta)
	 *  |Forward| and |ToHit| are Normalized Magnitudes (Value = 1) so 1 * 1 * cos(theta)
	 *	So it's the same as: Forward * ToHit = cos(theta)
	 */
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Now we get the Inverse Cosine of Theta (arc-cosine) to get Theta.
	double Theta = FMath::Acos(CosTheta);
	// The angle is in Radians so we'll convert it to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0)
		Theta *= -1.f;

	// GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Angle is %f"), Theta));
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 50.f, 5.f,
	// 									 FColor::Red, 5.f, 2.f);
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 50.f, 5.f,
	// 									 FColor::Green, 5.f, 2.f);
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 75.f, 5.f,
	// 									 FColor::Blue, 5.f, 2.f);

	return Theta;
}

void AEnemy::GetHitReactMontageSection(const double ImpactAngle, FName& Section)
{
	Section = {};
	if (ImpactAngle >= -45.f && ImpactAngle < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (ImpactAngle >= -135.f && ImpactAngle < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (ImpactAngle >= 45.f && ImpactAngle < 135.f)
	{
		Section = FName("FromRight");
	}
	else
	{
		Section = FName("FromBack");
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	HealthBarWidget->SetVisibility(true);
	if (AttributeComponent && AttributeComponent->IsAlive())
	{
		const double ImpactAngle = CalculateImpactAngle(ImpactPoint);
		FName Section;
		GetHitReactMontageSection(ImpactAngle, Section);
		PlayMontage(Section, HitReactMontage);
	}
	else
	{
		Die();
	}
		

	UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
}

float AEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	if (AttributeComponent && HealthBarWidget)
	{
		AttributeComponent->ReceiveDamage(Damage);
		HealthBarWidget->SetHealthPercent(AttributeComponent->GetCurrentHealthPercent());
	}
	CombatTarget = EventInstigator->GetPawn();

	return Damage;
}
