// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ActionRPGHUD.h"

#include "Blueprint/UserWidget.h"
#include "HUD/ActionRPGOverlay.h"

void AActionRPGHUD::BeginPlay()
{
    Super::BeginPlay();
    ActionRPGOverlay = CreateWidget<UActionRPGOverlay>(GetWorld()->GetFirstPlayerController(), ActionRPGOverlayClass);
    ActionRPGOverlay->AddToViewport();
}
