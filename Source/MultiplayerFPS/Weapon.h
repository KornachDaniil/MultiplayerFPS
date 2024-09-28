// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "MultiplayerFPS.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	
	UFUNCTION(Server, Reliable)
	void ServerStartFire();

	UFUNCTION(Server, Reliable)
	void ServerStopFire();

	EAmmoType GetAmmoType() const { return AmmoType; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	EAmmoType AmmoType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	EWeaponFireMode FireMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float HitscanRange = 9999.9f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float HitscanDamage = 20.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float FireRate = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UAnimMontage* FireAnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	USoundBase* NoAmmoSound;

	UPROPERTY()
	AFPSCharacter* Character;
	
	FTimerHandle FireTimer;

	bool bWantsFire;

	virtual void SetOwner(AActor* NewOwner) override;

	void StartFire();
	
	void FireHitscan(FVector FireLocation, FVector FireDirection);
	
private:	
	
};
