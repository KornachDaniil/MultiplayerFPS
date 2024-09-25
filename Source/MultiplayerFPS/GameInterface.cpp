// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInterface.h"

#include "Components/TextBlock.h"

void UGameInterface::UpdateVarHealth(FText Health) const
{
	VarHealth->SetText(Health);
}

void UGameInterface::UpdateValArmor(FText Armor) const
{
	VarArmor->SetText(Armor);
}
