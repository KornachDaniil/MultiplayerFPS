// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Pickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API APickup : public AActor
{
	GENERATED_BODY()
	
protected:
	APickup();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnPickup(AFPSCharacter* Character);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	void SetActive(bool bNewIPickuped);
	void Respawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	USoundBase* PickupSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	URotatingMovementComponent* RotatingMovement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	float RespawnTime = 30.0f;
	

	FTimerHandle RespawnHandle;

	bool bIsPickuped = false;
};
