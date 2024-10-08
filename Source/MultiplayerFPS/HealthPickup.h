// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AHealthPickup : public APickup
{
	GENERATED_BODY()
public:
	virtual void OnPickup(AFPSCharacter* Character) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	float Health;
};
