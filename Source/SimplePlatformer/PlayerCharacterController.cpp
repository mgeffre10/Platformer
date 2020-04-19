// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "Blueprint/UserWidget.h"

void APlayerCharacterController::BeginPlay()
{

	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);

		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	
}