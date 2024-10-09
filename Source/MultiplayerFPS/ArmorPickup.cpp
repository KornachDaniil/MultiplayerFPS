// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorPickup.h"

void AArmorPickup::OnPickup(AFPSCharacter* Character)
{
	Super::OnPickup(Character);

	Character->AddArmor(Armor);
}
