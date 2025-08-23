// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Treasure.h"

#include "Characters/ActionRPGCharacter.h"
#include "Interfaces/IPluginManager.h"


// Sets default values
ATreasure::ATreasure()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATreasure::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
	{
		PickupInterface->AddGold(this);
		SpawnPickupSound();
		Destroy();
	}
}
