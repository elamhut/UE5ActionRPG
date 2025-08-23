// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ACTIONRPG_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	[[nodiscard]] FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	[[nodiscard]] FORCEINLINE float GetCurrentHealthPercent() const { return CurrentHealth / MaxHealth; }
	[[nodiscard]] FORCEINLINE float GetStaminaPercent() const { return CurrentStamina / MaxStamina; }
	[[nodiscard]] FORCEINLINE float GetDodgeCost() const { return DodgeStaminaCost; }
	[[nodiscard]] FORCEINLINE float GetStamina() const { return CurrentStamina; }
	[[nodiscard]] FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	[[nodiscard]] FORCEINLINE int32 GetGold() const { return Gold; }
	[[nodiscard]] FORCEINLINE int32 GetSouls() const { return Souls; }
	
	FORCEINLINE bool IsAlive() const { return CurrentHealth > 0.f; };

	void AddGold(int32 GoldCount);
	void AddSouls(int32 SoulCount);
	void ReceiveDamage(const float Damage);
	void UseStamina(const float StaminaCost);
	void RegenerateStamina(float DeltaTime);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float CurrentStamina;
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float DodgeStaminaCost{10.f};
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float StaminaRegenRate{3.f};

	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	int32 Gold;
	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	int32 Souls;
};
