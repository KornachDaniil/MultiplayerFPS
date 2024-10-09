// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"

void AAmmoPickup::OnPickup(AFPSCharacter* Character)
{
	Super::OnPickup(Character);

	Character->AddAmmo(AmmoType, AmmoCount);
}
