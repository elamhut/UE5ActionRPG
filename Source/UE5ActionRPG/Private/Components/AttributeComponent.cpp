// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"


UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UAttributeComponent::RegenerateStamina(float DeltaTime) { CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina); }

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RegenerateStamina(DeltaTime);
}

void UAttributeComponent::AddGold(int32 GoldCount)
{
	Gold += GoldCount;
}
void UAttributeComponent::AddSouls(int32 SoulCount)
{
	Souls += SoulCount;
}

void UAttributeComponent::ReceiveDamage(const float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
}

void UAttributeComponent::UseStamina(const float StaminaCost)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - StaminaCost, 0.f, MaxStamina);
}

