// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"


class UHealthBar;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ACTIONRPG_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UHealthBarComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetHealthPercent(float Percent);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UHealthBar> HealthBarWidget;
};
