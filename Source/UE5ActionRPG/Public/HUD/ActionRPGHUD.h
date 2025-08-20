// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ActionRPGHUD.generated.h"

class UActionRPGOverlay;
/**
 * 
 */
UCLASS()
class UE5ACTIONRPG_API AActionRPGHUD : public AHUD
{
    GENERATED_BODY()

public:
    FORCEINLINE TObjectPtr<UActionRPGOverlay> GetOverlay() const { return ActionRPGOverlay; }
    
protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UActionRPGOverlay> ActionRPGOverlayClass;

    UPROPERTY()
    TObjectPtr<UActionRPGOverlay> ActionRPGOverlay;
};
