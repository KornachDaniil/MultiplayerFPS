// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSMultiplayer_PlayerController.h"

void AFPSMultiplayer_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == true && PlayerMenuClass != nullptr)
	{
		PlayerMenu = CreateWidget<UPlayerMenu>(this, PlayerMenuClass);

		if (PlayerMenu != nullptr)
		{
			PlayerMenu->AddToViewport(0);
			PlayerMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AFPSMultiplayer_PlayerController::OpenTab() const
{
	if (PlayerMenu != nullptr)
	{
		PlayerMenu->SetVisibility(ESlateVisibility::Visible);
	}
}

void AFPSMultiplayer_PlayerController::CloseTab() const
{
	if (PlayerMenu != nullptr)
	{
		PlayerMenu->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AFPSMultiplayer_PlayerController::ToggleScoreboard() const // ХЗ нахуя это
{
	if (PlayerMenu != nullptr)
	{
		PlayerMenu->ToggleScoreboard();
	}
}

void AFPSMultiplayer_PlayerController::ClientNotifyKill_Implementation(const FString& Name)
{
	if (PlayerMenu != nullptr)
	{
		PlayerMenu->NotifyKill(Name);
	}
}

void AFPSMultiplayer_PlayerController::ClientShowScoreboard_Implementation() // ХЗ нахуя это
{
	if (PlayerMenu != nullptr)
	{
		PlayerMenu->SetScoreboardVisibility(true);
	}
}
