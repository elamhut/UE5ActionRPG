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
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void ExecuteGetHit(const FHitResult& HitResult, AActor* Actor);
	bool IsActorSameType(AActor* Actor);
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	void DisableSphereCollision();
	void DeactivateEmberParticles();

	[[nodiscard]] FORCEINLINE  UBoxComponent* GetWeaponCollider() const { return  WeaponCollider; }
	
	TArray<AActor*> IgnoreActors;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
	
private:
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	FVector BoxTraceExtent{20.f};

	UPROPERTY(EditAnywhere)
	bool bShowBoxDebug{false};
	
	UPROPERTY(EditAnywhere)
	float Damage{20.f};
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<UBoxComponent> WeaponCollider;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> TraceStart;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> TraceEnd;


};
