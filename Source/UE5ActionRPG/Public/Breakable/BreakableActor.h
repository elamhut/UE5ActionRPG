// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"

#include "BreakableActor.generated.h"

class UCapsuleComponent;
struct FChaosBreakEvent;
class ATreasure;

UCLASS()
class UE5ACTIONRPG_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABreakableActor();
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	UFUNCTION()
	void DropItem(const FChaosBreakEvent& Event);
	

protected:
	bool bBroken = false;
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;
	
private:
	UPROPERTY(EditAnywhere, Category="BreakableProperties")
	TArray<TSoftClassPtr<ATreasure>> ArrayOfTreasures;
	
};
