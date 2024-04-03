#pragma once

UENUM(BlueprintType, meta =(Bitflags, UseEnumValuesAsMaskValuesInEditor ="true"))
enum class EItemClassification : uint8
{
	None = 0 UMETA(Hidden),
	Machine = 1 << 0,
	Ammo = 1 << 1,
	Weapon = 1 << 2,
	Placeable = 1 << 3,
	CraftingMaterial = 1 << 4,
	Tool = 1 << 5,
	Mountable = 1 << 6
};

ENUM_CLASS_FLAGS(EItemClassification);
