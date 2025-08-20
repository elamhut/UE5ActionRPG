// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ActionRPGOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UActionRPGOverlay::SetHealthBarPercent(float Percent)
{
    if (HealthProgressBar)
        HealthProgressBar->SetPercent(Percent);
}

void UActionRPGOverlay::SetStaminaBarPercent(float Percent)
{
    if (StaminaProgressBar)
        StaminaProgressBar->SetPercent(Percent);
}

void UActionRPGOverlay::SetGold(int32 Gold)
{
    if (GoldText)
    {
        GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
    }
}
void UActionRPGOverlay::SetSouls(int32 Souls)
{
    if (SoulsText)
    {
        SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
    }
}
