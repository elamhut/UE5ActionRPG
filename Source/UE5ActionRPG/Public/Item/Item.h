// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class UE5ACTIONRPG_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Speed{5.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude{0.25f};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> Particles;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	
	EItemState ItemState = EItemState::EIS_Hovering;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Transform Parameters")
	float TransformedSin();
	
	UFUNCTION(BlueprintPure, Category = "Transform Parameters")
	float TransformedCos();
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp,
	                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();

	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sine Parameters", meta = (AllowPrivateAccess = "true"))
	float RunningTime{0.f};

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> PickupSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> PickupParticle;
};
