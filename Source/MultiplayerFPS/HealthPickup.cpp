// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

void AHealthPickup::OnPickup(AFPSCharacter* Character)
{
	Super::OnPickup(Character);

	Character->AddHealth(Health);
}
