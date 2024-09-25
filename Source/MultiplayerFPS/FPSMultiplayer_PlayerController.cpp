// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSMultiplayer_PlayerController.h"

#include "GameInterface.h"

void AFPSMultiplayer_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController() == true)
	{
		if (BP_GameInterface != nullptr)
		{
			GameInterface = CreateWidget<UGameInterface>(this, BP_GameInterface);
			GameInterface->AddToViewport();
		}
	}
}

void AFPSMultiplayer_PlayerController::UpdateVarHealth(const FText& Health) const
{
	GameInterface->UpdateVarHealth(Health);
}

void AFPSMultiplayer_PlayerController::UpdateVarArmor(const FText& Armor) const
{
	GameInterface->UpdateValArmor(Armor);
}	