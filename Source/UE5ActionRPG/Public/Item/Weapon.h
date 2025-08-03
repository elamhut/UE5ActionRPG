// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class UE5ACTIONRPG_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	void Equip(USceneComponent* InParent, FName InSocketName);
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);

	[[nodiscard]] FORCEINLINE  UBoxComponent* GetWeaponCollider() const { return  WeaponCollider; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<UBoxComponent> WeaponCollider;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> TraceStart;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> TraceEnd;
};
