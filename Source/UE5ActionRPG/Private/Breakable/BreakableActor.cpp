// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"

#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

#include "Item/Treasure.h"


ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollectionComponent);
	GeometryCollectionComponent->SetGenerateOverlapEvents(true);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// This event is very dumb, it plays once for each piece broken in this event... Great job.
	GeometryCollectionComponent->SetNotifyBreaks(true);
	GeometryCollectionComponent->OnChaosBreakEvent.AddUniqueDynamic(this, &ABreakableActor::DropItem);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(GetRootComponent());
}

void ABreakableActor::DropItem(const FChaosBreakEvent& Event)
{
	// This function plays one time for each fucking Notification played for Break Events,
	// it plays one notification for EACH PIECE in a Destructible...
	// This is why the boolean is there, bro.
	UE_LOG(LogTemp, Warning, TEXT("PLAYED EVENT, DID IT SPAWN? BROKEN IS %s"), *LexToString(bBroken));
	
	if (!bBroken)
		bBroken = true;
	else return;

	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	const int32 Selection = FMath::RandRange(0,ArrayOfTreasures.Num() - 1);
	GetWorld()->SpawnActor<ATreasure>(ArrayOfTreasures[Selection].Get(), GetActorLocation(), GetActorRotation());
	
	UE_LOG(LogTemp, Warning, TEXT("SPAWNED THIS BITCH!"));
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	
}


void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}
