// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FPSCharacter.generated.h"

class UCameraComponent;

UCLASS()
class MULTIPLAYERFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FollowCamera;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void SetHealth(float Amount) { Health = Amount; }
	void AddHealth(float Amount) { SetHealth(Health + Amount); }
	UFUNCTION(BlueprintCallable)
	void RemoveHealth(float Amount) { SetHealth(Health - Amount); }
	bool IsDead() const { return Health == 0.0f; }

	void AddArmor(float Amount) { SetArmor(Armor + Amount); }
	void SetArmor(float Amount) { Armor = Amount; }
	void ArmorAbsorbDamage(float& Damage);
	
	UPROPERTY(EditAnywhere,Category="Setup")
	USoundBase* SpawnSound;
	
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

	UPROPERTY(EditAnywhere, Category="Input")
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, Category="Input")
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;
	
private:	
	

};
