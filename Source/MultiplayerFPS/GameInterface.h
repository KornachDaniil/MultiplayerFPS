// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameInterface.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UGameInterface : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* VarHealth;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* VarArmor;
	
	void UpdateVarHealth(FText Health) const;
	void UpdateValArmor(FText Armor) const;
};
