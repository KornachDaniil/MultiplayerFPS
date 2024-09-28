// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ENUM_TO_INT32(Value) static_cast<int32>(Value)
#define GET_WRAPPED_ARRAY_INDEX(Index, Count) (Index % Count + Count) % Count

UENUM()
enum class EWeaponType : uint8
{
	Pistol,
	MachineGun,
	Railgun,
	MAX
};

UENUM()
enum class EWeaponFireMode : uint8
{
	Single,
	Automatic
};

UENUM()
enum class EAmmoType : uint8
{
	PistolBullets,
	MachineGunBullets,
	Slugs,
	MAX
};
