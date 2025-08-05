// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


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
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::PlayHitReactMontage(const FName& SectionName) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

double AEnemy::CalculateImpactAngle(const FVector& ImpactPoint) {
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

void AEnemy::GetHitReactMontageSection(const double ImpactAngle, FName& Section) {
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
	const double ImpactAngle = CalculateImpactAngle(ImpactPoint);
	FName Section;
	GetHitReactMontageSection(ImpactAngle, Section);
	PlayHitReactMontage(Section);

	UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);

}
