// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AAmmoPickup : public APickup
{
	GENERATED_BODY()
public:
	virtual void OnPickup(AFPSCharacter* Character) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 AmmoCount;
	
};
