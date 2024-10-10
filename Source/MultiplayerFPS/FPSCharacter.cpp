// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapon.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->bUsePawnControlRotation = true;
	FollowCamera->SetupAttachment(GetMesh(), "Camera");
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
}

void AFPSCharacter::ApplyDamage(float Damage, AFPSCharacter* DamageCauser)
{
	if (IsDead() == true)
	{
		return;
	}

	ArmorAbsorbDamage(Damage);
	RemoveHealth(Damage);

	if (HitSound != nullptr && DamageCauser != nullptr)
	{
		DamageCauser->ClientPlaySound2D(HitSound);
	}

	if (IsDead())
	{
		GameMode->OnKill(DamageCauser->GetController(), GetController());
	}
	else
	{
		if (DamageSound != nullptr)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), DamageSound);
		}
	}
}

bool AFPSCharacter::AddWeapon(EWeaponType WeaponType)
{
	const int32 NewWeaponIndex = ENUM_TO_INT32(WeaponType);

	if (!WeaponClasses.IsValidIndex(NewWeaponIndex) || Weapons[NewWeaponIndex] != nullptr)
	{
		return false;
	}

	UClass* WeaponClass = WeaponClasses[NewWeaponIndex];

	if (WeaponClass == nullptr)
	{
		return false;
	}

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParameters);

	if (NewWeapon == nullptr)
	{
		return false;
	}

	NewWeapon->SetActorHiddenInGame(true);
	Weapons[NewWeaponIndex] = NewWeapon;
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
	return true;
}

int32 AFPSCharacter::GetWeaponAmmo() const
{
	return Weapon != nullptr ? Ammo[ENUM_TO_INT32(Weapon->GetAmmoType())] : 0;
}

void AFPSCharacter::ClientPlaySound2D_Implementation(USoundBase* Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

void AFPSCharacter::MulticastPlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}


// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);
	if (HasAuthority() == false)
	{
		return;
	}
	SetHealth(50.0f);

	constexpr int32 WeaponCount = ENUM_TO_INT32(EWeaponType::MAX);

	Weapons.Init(nullptr, WeaponCount);

	constexpr int32 AmmoCount = ENUM_TO_INT32(EAmmoType::MAX);

	Ammo.Init(50, AmmoCount);

	for (int32 i = 0; i < WeaponCount; i++)
	{
		AddWeapon(static_cast<EWeaponType>(i));
	}

	EquipWeapon(EWeaponType::MachineGun, false);

	GameMode = Cast<AMultiplayerFPSGameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (EnhancedPlayerInputComponent != nullptr && PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (EnhancedSubsystem != nullptr)
		{
			EnhancedSubsystem->AddMappingContext(DefaultMappingContext, 1);
		}
		
		EnhancedPlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
		EnhancedPlayerInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
		EnhancedPlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedPlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedPlayerInputComponent->BindAction(FireInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::StartFire);
		EnhancedPlayerInputComponent->BindAction(FireInputAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopFire);
		EnhancedPlayerInputComponent->BindAction(PistolInputAction, ETriggerEvent::Started, this, &AFPSCharacter::Pistol);
		EnhancedPlayerInputComponent->BindAction(MachineGunInputAction, ETriggerEvent::Started, this, &AFPSCharacter::MachineGun);
		EnhancedPlayerInputComponent->BindAction(RailgunInputAction, ETriggerEvent::Started, this, &AFPSCharacter::Railgun);
		EnhancedPlayerInputComponent->BindAction(PreviousWeaponInputAction, ETriggerEvent::Started, this, &AFPSCharacter::PreviousWeapon);
		EnhancedPlayerInputComponent->BindAction(NextWeaponInputAction, ETriggerEvent::Started, this, &AFPSCharacter::NextWeapon);
	}
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(AFPSCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Weapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Weapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Ammo, COND_OwnerOnly);
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
    	FVector2D MovementVector = Value.Get<FVector2D>();
    
    	if (Controller != nullptr)
    	{
    		// add movement 
    		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
    		AddMovementInput(GetActorRightVector(), MovementVector.X);
    	}
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
    	FVector2D LookAxisVector = Value.Get<FVector2D>();
    
    	if (Controller != nullptr)
    	{
    		// add yaw and pitch input to controller
    		AddControllerYawInput(LookAxisVector.X);
    		AddControllerPitchInput(LookAxisVector.Y);
    	}
}

void AFPSCharacter::StartFire(const FInputActionValue& Value)
{
	if (Weapon != nullptr)
	{
		Weapon->ServerStartFire();
	}
}

void AFPSCharacter::StopFire(const FInputActionValue& Value)
{
	if (Weapon != nullptr)
	{
		Weapon->ServerStopFire();
	}
}

void AFPSCharacter::Pistol(const FInputActionValue& Value)
{
	ServerEquipWeapon(EWeaponType::Pistol);
}

void AFPSCharacter::MachineGun(const FInputActionValue& Value)
{
	ServerEquipWeapon(EWeaponType::MachineGun);
}

void AFPSCharacter::Railgun(const FInputActionValue& Value)
{
	ServerEquipWeapon(EWeaponType::Railgun);
}

void AFPSCharacter::PreviousWeapon(const FInputActionValue& Value)
{
	ServerCycleWeapons(-1);
}

void AFPSCharacter::NextWeapon(const FInputActionValue& Value)
{
	ServerCycleWeapons(1);
}

void AFPSCharacter::ServerCycleWeapons_Implementation(int32 Direction)
{
	const int32 WeaponCount = Weapons.Num();
	const int32 StartWeaponIndex = GET_WRAPPED_ARRAY_INDEX(WeaponIndex + Direction, WeaponCount);

	for (int32 i = StartWeaponIndex; i != WeaponIndex; i = GET_WRAPPED_ARRAY_INDEX(i + Direction, WeaponCount))
	{
		if (EquipWeapon(static_cast<EWeaponType>(i)))
		{
			break;
		}
	}
}

void AFPSCharacter::ServerEquipWeapon_Implementation(EWeaponType WeaponType)
{
	EquipWeapon(WeaponType);
}

bool AFPSCharacter::EquipWeapon(EWeaponType WeaponType, bool bPlaySound)
{
	const int32 NewWeaponIndex = ENUM_TO_INT32(WeaponType);

	if (!Weapons.IsValidIndex(NewWeaponIndex))
	{
		return false;
	}

	AWeapon* NewWeapon = Weapons[NewWeaponIndex];

	if (NewWeapon == nullptr || Weapon == NewWeapon)
	{
		return false;
	}

	if (Weapon != nullptr)
	{
		Weapon->SetActorHiddenInGame(true);
	}

	Weapon = NewWeapon;
	Weapon->SetActorHiddenInGame(false);

	WeaponIndex = NewWeaponIndex;

	if (WeaponChangeSound != nullptr && bPlaySound)
	{
		ClientPlaySound2D(WeaponChangeSound);
	}

	return true;
}

void AFPSCharacter::ArmorAbsorbDamage(float& Damage)
{
	const float AbsorbedDamage = Damage * ArmorAbsorption;
	const float RemainingArmor = Armor - AbsorbedDamage;

	SetArmor(RemainingArmor);

	Damage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmor, 0.0f);
}
