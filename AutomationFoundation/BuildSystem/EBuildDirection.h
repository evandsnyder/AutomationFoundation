#pragma once

#include "EBuildDirection.generated.h"

UENUM(BlueprintType)
enum class EBuildDirection : uint8
{
	Unspecified = 0,
	Input = 1,
	Output = 2
};
