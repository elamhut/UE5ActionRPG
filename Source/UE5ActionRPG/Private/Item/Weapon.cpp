// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollier"));
    WeaponCollider->SetupAttachment(GetRootComponent());
    WeaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
    WeaponCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

    TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
    TraceStart->SetupAttachment(GetRootComponent());

    TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
    TraceEnd->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    WeaponCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName InSocketName)
{
    const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::DisableSphereCollision()
{
    if (Sphere)
    {
        Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AWeapon::DeactivateEmberParticles() { Particles.Get()->Deactivate(); }

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
    ItemState = EItemState::EIS_Equipped;
    SetOwner(NewOwner);
    SetInstigator(NewInstigator);
    AttachMeshToSocket(InParent, InSocketName);
    DisableSphereCollision();

    DeactivateEmberParticles();
}

void AWeapon::ExecuteGetHit(FHitResult HitResult, AActor* Actor)
{
    if (IHitInterface* HitActor = Cast<IHitInterface>(Actor))
    {
        HitActor->Execute_GetHit(Actor, HitResult.ImpactPoint);
    }
}

bool AWeapon::IsActorSameType(AActor* Actor) { return GetOwner()->ActorHasTag(TEXT("Enemy")) && Actor->ActorHasTag(TEXT("Enemy")); }

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (IsActorSameType(OtherActor)) return;
    
    FHitResult HitResult;
    BoxTrace(HitResult);

    if (AActor* Actor = HitResult.GetActor())
    {
        if (IsActorSameType(Actor)) return;
        
        UGameplayStatics::ApplyDamage(Actor, Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
        ExecuteGetHit(HitResult, Actor);
        UE_LOG(LogTemp, Warning, TEXT("BATEU NO ACTOR %s"), *Actor->GetName());
        CreateFields(HitResult.ImpactPoint);
    }
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
    const FVector Start = TraceStart->GetComponentLocation();
    const FVector End   = TraceEnd->GetComponentLocation();

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    ActorsToIgnore.Add(GetOwner());
    for (AActor* Actor : IgnoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }

    UKismetSystemLibrary::BoxTraceSingle(
        this,
        Start,
        End,
        BoxTraceExtent,
        TraceStart->GetComponentRotation(),
        TraceTypeQuery1,
        false,
        ActorsToIgnore,
        bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
        BoxHit,
        true 
    );

    IgnoreActors.AddUnique(BoxHit.GetActor());
}
