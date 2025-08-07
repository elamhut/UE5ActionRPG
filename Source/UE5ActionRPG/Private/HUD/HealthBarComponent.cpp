// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "HUD/HealthBar.h"


// Sets default values for this component's properties
UHealthBarComponent::UHealthBarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (!HealthBarWidget)
		HealthBarWidget = CastChecked<UHealthBar>(GetUserWidgetObject());

	if (UProgressBar* ProgressBar = HealthBarWidget->HealthBar.Get())
		ProgressBar->SetPercent(Percent);
		
	
}

