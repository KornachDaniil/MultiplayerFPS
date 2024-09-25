// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSMultiplayer_PlayerController.generated.h"

class UGameInterface;
/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AFPSMultiplayer_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="UserInterface")
	TSubclassOf<UGameInterface> BP_GameInterface;
	
	void UpdateVarHealth(const FText& Health) const;
	void UpdateVarArmor(const FText& Armor) const;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UGameInterface* GameInterface;
	
};
