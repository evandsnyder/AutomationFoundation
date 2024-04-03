#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ECraftingMachineType
{
	Unknown = 0,
	Basic,
	Chemical,
	ResourceExtractor,
	Smelter
	// Others.. ?
};

UENUM()
enum class ECraftingMachineFilterType
{
	All,
	Basic,
	Chemical,
	ResourceExtractor,
	Smelter,
	InventoryOnly
};