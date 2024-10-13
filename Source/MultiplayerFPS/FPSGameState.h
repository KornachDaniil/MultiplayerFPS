// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSPlayerState.h"
#include "GameFramework/GameState.h"
#include "FPSGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AFPSGameState : public AGameState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<AFPSPlayerState*> GetPlayerStatesOrderedByKills() const;
	
	
	FORCEINLINE void SetKillLimit(int32 NewKillLimit) { KillLimit = NewKillLimit; }
	FORCEINLINE int32 GetKillLimit() const { return KillLimit; }
protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category="GameState")
	int32 KillLimit;
};
