// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

void AWeaponPickup::OnPickup(AFPSCharacter* Character)
{
	Super::OnPickup(Character);

	bool bNotWeaponsFull = Character->AddWeapon(WeaponType);
	if (bNotWeaponsFull == false)
	{
		Character->AddAmmo(AmmoType, AmmoCount);
	}
	else
	{
		Character->ServerEquipWeapon(WeaponType);
	}
}
