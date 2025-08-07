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
	
	[[nodiscard]] FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	void ReceiveDamage(const float Damage);
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxHealth;
};
