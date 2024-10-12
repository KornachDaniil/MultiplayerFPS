// Copyright Epic Games, Inc. All Rights Reserved.


#include "MultiplayerFPSGameModeBase.h"

#include "FPSCharacter.h"
#include "FPSGameState.h"
#include "FPSPlayerState.h"

void AMultiplayerFPSGameModeBase::OnKill(const AController* KillerController, AController* VictimController)
{
	if (HasWinner() == true)
	{
		return;
	}
	
	if (KillerController != nullptr && KillerController != VictimController)
	{
		AFPSPlayerState* KillerState = Cast<AFPSPlayerState>(KillerController->PlayerState);
		if (KillerState != nullptr)
		{
			KillerState->AddKills();
		}
	}

	if (VictimController != nullptr)
	{
		AFPSPlayerState* VictimState = Cast<AFPSPlayerState>(VictimController->PlayerState);
		if (VictimState != nullptr)
		{
			VictimState->AddDeaths();
		}

		AFPSCharacter* VictimCharacter = Cast<AFPSCharacter>(VictimController->GetCharacter());
		if (VictimCharacter != nullptr)
		{
			VictimCharacter->Destroy();
			RestartPlayer(VictimController);
		}
	}
}

bool AMultiplayerFPSGameModeBase::HasWinner() const
{
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		const AFPSPlayerState* FPSPlayerState = Cast<AFPSPlayerState>(PlayerState);
		if (FPSPlayerState != nullptr && KillLimit == FPSPlayerState->GetKills())
		{
			return true;
		}
	}
	return false;
}

void AMultiplayerFPSGameModeBase::RestartMap() const
{
	GetWorld()->ServerTravel(GetWorld()->GetName(), false, false);
}

bool AMultiplayerFPSGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
{
	return Super::ShouldSpawnAtStartSpot(Player);
}

void AMultiplayerFPSGameModeBase::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	AFPSGameState* FPSGameState = Cast<AFPSGameState>(GameState);
	if (FPSGameState != nullptr)
	{
		FPSGameState->SetKillLimit(KillLimit);
	}
}

void AMultiplayerFPSGameModeBase::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiplayerFPSGameModeBase::RestartMap, 5.0f);
}

bool AMultiplayerFPSGameModeBase::ReadyToEndMatch_Implementation()
{
	return HasWinner() == true;
}
