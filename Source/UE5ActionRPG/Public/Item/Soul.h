// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Soul.generated.h"

class UNiagaraSystem;

UCLASS()
class UE5ACTIONRPG_API ASoul : public AItem
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASoul();
    virtual void Tick(float DeltaTime) override;
	[[nodiscard]] FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void SetSouls(const int32 SoulsCount) { Souls += SoulsCount; }
    

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
    UPROPERTY(EditAnywhere, Category="Soul Property")
    int32 Souls;
};
