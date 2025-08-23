// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/Weapon.h"
#include "Kismet/GameplayStatics.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter)
	{
		const double ImpactAngle = CalculateImpactAngle(Hitter->GetActorLocation());
		PlayMontageSection(GetHitReactMontageSection(ImpactAngle), HitReactMontage);
	}
	else
		Die();

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void ABaseCharacter::DoAttack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
		CombatTarget = nullptr;
}

void ABaseCharacter::Die()
{
	Tags.Add(FName("Dead"));
    PlayDeathMontage();
}


void ABaseCharacter::HandleDamage(float DamageAmount)
{
	AttributeComponent->ReceiveDamage(DamageAmount);
}

bool ABaseCharacter::CanAttack() { return false; }

double ABaseCharacter::CalculateImpactAngle(const FVector& ImpactPoint) const
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

FName ABaseCharacter::GetHitReactMontageSection(double ImpactAngle)
{
	FName Section;
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

	return Section;
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontageSections, AttackMontage);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontageSections, DeathMontage);
	TEnumAsByte<EDeathPose> Pose(Selection);

	if (Pose < EDeathPose::EDP_MAX)
		DeathPose = Pose;
	
	return Selection;
}

void ABaseCharacter::PlayDodgeMontage()
{
	PlayMontageSection(FName("Default"), DodgeMontage);
}

void ABaseCharacter::PlayMontageSection(const FName& SectionName, UAnimMontage* Montage) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(const TArray<FName>& SectionNames, UAnimMontage* Montage) const
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(SectionNames[Selection], Montage);

	return Selection;
}

void ABaseCharacter::StopAttackMontage()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (!CombatTarget) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
		return CombatTarget->GetActorLocation();

	return FVector();
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
}

bool ABaseCharacter::IsAlive()
{
	return AttributeComponent && AttributeComponent->IsAlive();
}

void ABaseCharacter::AttackEnd() {}


void ABaseCharacter::SetWeaponCollisionType(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponCollider())
	{
		EquippedWeapon->GetWeaponCollider()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
