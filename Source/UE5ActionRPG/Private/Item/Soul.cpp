﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Soul.h"

#include "NiagaraFunctionLibrary.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASoul::ASoul()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASoul::BeginPlay()
{
    Super::BeginPlay();
    
}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
    {
        PickupInterface->AddSouls(this);
        
        SpawnPickupSystem();
        SpawnPickupSound();
        
        Destroy();
    }

}

// Called every frame
void ASoul::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

