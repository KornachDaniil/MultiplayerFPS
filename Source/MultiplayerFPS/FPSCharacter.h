// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "MultiplayerFPS.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UCameraComponent;
class AWeapon;

UCLASS()
class MULTIPLAYERFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();
	
	void ApplyDamage(float Damage, AFPSCharacter* DamageCauser);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAnimMontage(UAnimMontage* AnimMontage);

	UFUNCTION(Client, Unreliable)
	void ClientPlaySound2D(USoundBase* Sound);

	void AddWeapon(EWeaponType WeaponType);

	UFUNCTION(BlueprintCallable, Category = "FPS Character")
	int32 GetWeaponAmmo() const;

	void AddAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) + Amount); }
	void ConsumeAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) - Amount); }
	int32 GetAmmo(EAmmoType AmmoType) const { return Ammo[ENUM_TO_INT32(AmmoType)]; }
	void SetAmmo(EAmmoType AmmoType, int32 Amount) { Ammo[ENUM_TO_INT32(AmmoType)] = FMath::Max(0, Amount); }

	FORCEINLINE FVector GetCameraLocation() const { return FollowCamera->GetComponentLocation(); }
	FORCEINLINE FVector GetCameraDirection() const { return  GetControlRotation().Vector(); }

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FollowCamera;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Функции для управления персонажем 
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartFire(const FInputActionValue& Value);
	void StopFire(const FInputActionValue& Value);
	void Pistol(const FInputActionValue& Value);
	void MachineGun(const FInputActionValue& Value);
	void Railgun(const FInputActionValue& Value);
	void PreviousWeapon(const FInputActionValue& Value);
	void NextWeapon(const FInputActionValue& Value);

	// Серверные функции
	UFUNCTION(Server, Reliable)
	void ServerCycleWeapons(int32 Direction);
	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(EWeaponType WeaponType);

	bool EquipWeapon(EWeaponType WeaponType, bool bPlaySound = true);

	//  Функции связанные с Health
	UFUNCTION(BlueprintCallable)
	void SetHealth(float Amount) { Health = Amount; }
	void AddHealth(float Amount) { SetHealth(Health + Amount); }
	UFUNCTION(BlueprintCallable)
	void RemoveHealth(float Amount) { SetHealth(Health - Amount); }
	bool IsDead() const { return Health == 0.0f; }

	// Функции связанные с Armor
	void AddArmor(float Amount) { SetArmor(Armor + Amount); }
	void SetArmor(float Amount) { Armor = Amount; }
	void ArmorAbsorbDamage(float& Damage);

	// Броня и хп персонажа
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category="Health")
	float Health;
	UPROPERTY(Replicated, EditAnywhere,  BlueprintReadOnly, Category="Health")
	float Armor;
	UPROPERTY(EditAnywhere,  BlueprintReadOnly, Category="Health")
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere,  BlueprintReadOnly, Category="Health")
	float MaxArmor = 100;
	UPROPERTY(EditAnywhere,  BlueprintReadOnly, Category="Health")
	float ArmorAbsorption = 0.5;

	// EnhancedInput
	UPROPERTY(EditAnywhere, Category="Input")
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, Category="Input")
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* FireInputAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* PistolInputAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* MachineGunInputAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* RailgunInputAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* PreviousWeaponInputAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* NextWeaponInputAction; 

	// Все что связано с AWeapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FPS Character")
	USoundBase* SpawnSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FPS Character")
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FPS Character")
	USoundBase* WeaponChangeSound;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="FPS Character")
	TArray<int32> Ammo;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="FPS Character")
	TArray<class AWeapon*> Weapons;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FPS Character")
	TArray<TSubclassOf<AWeapon>> WeaponClasses;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="FPS Character")
	AWeapon* Weapon;

	int32 WeaponIndex = INDEX_NONE;
	
private:	
	

};
