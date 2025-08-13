// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Item/Weapon.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::DoAttack() {}
void ABaseCharacter::Die() {}
void ABaseCharacter::PlayAttackMontage() {}
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

void ABaseCharacter::PlayMontage(const FName& SectionName, UAnimMontage* Montage) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void ABaseCharacter::AttackEnded(UAnimMontage* Montage, bool bInterrupted) {}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetWeaponCollisionType(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponCollider())
	{
		EquippedWeapon->GetWeaponCollider()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
