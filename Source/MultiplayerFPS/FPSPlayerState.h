// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AFPSPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	void AddKills() { Kills++; }
	void AddDeaths() { Deaths++; }
	int32 GetKills() const { return Kills; }
	int32 GetDeath() const { return Deaths; }
	
protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Kills;
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Deaths;
};
