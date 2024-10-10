// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerFPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSGameModeBase : public AGameMode
{
	GENERATED_BODY()
public:
	void OnKill(const AController* KillerController, AController* VictimController);
	
protected:
	bool HasWinner() const;

	void RestartMap() const;
	
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual bool ReadyToEndMatch_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 KillLimit = 10;
};
