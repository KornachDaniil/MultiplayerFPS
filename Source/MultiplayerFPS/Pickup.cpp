// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APickup::APickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName("OverlapAll");
	RootComponent = Mesh;

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0.0f, 90.0f, 0.0f);

	bReplicates = true;
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
}

// protected:
void APickup::OnPickup(AFPSCharacter* Character)
{
	if (PickupSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
	}
}

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	if (Character != nullptr && Character->IsDead() == false && HasAuthority() == true)
	{
		OnPickup(Character);

		SetActive(false);

		GetWorldTimerManager().SetTimer(RespawnHandle, this, &APickup::Respawn, RespawnTime);
	}
}

void APickup::SetActive(bool bNewIPickuped)
{
	SetActorHiddenInGame(!bIsPickuped);
	SetActorEnableCollision(bIsPickuped);
}

void APickup::Respawn()
{
	SetActive(true);
}

