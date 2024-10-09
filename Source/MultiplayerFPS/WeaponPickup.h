// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "WeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AWeaponPickup : public APickup
{
	GENERATED_BODY()
public:
	virtual void OnPickup(AFPSCharacter* Character) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	int32 AmmoCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	EWeaponType WeaponType;
	
};
