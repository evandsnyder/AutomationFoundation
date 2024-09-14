#pragma once

UENUM(BlueprintType, meta =(Bitflags, UseEnumValuesAsMaskValuesInEditor ="true"))
enum class EEquipType : uint8
{
	//None = 0 UMETA(Hidden),
	Unarmed = 0,
	// Unarmed =  << 0,
	Sidearm = 1 << 0,
	Longarm = 2 << 0,
	OneHandedMelee = 3 << 0,
	TwoHandedMelee = 4 << 0,
	OneHandedResource = 5 << 0,
	TwoHandedResource = 6 << 0
};

ENUM_CLASS_FLAGS(EEquipType);