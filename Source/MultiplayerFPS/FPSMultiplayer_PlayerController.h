// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMenu.h"
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
	void OpenTab() const;
	void CloseTab() const;

	void ToggleScoreboard() const; // ХЗ нахуя это
	UFUNCTION(Client, Reliable)
	void ClientNotifyKill(const FString& Name);
	UFUNCTION(Client, Reliable)
	void ClientShowScoreboard(); // ХЗ нахуя это

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlayerMenu> PlayerMenuClass; 
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UPlayerMenu* PlayerMenu;
};
