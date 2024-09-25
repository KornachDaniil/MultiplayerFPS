// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->bUsePawnControlRotation = true;
	FollowCamera->SetupAttachment(GetMesh(), "Camera");
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
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
	SetHealth(MaxHealth);
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
	}
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(AFPSCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Armor, COND_OwnerOnly);
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

void AFPSCharacter::ArmorAbsorbDamage(float& Damage)
{
	const float AbsorbedDamage = Damage * ArmorAbsorption;
	const float RemainingArmor = Armor - AbsorbedDamage;

	SetArmor(RemainingArmor);

	Damage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmor, 0.0f);
}