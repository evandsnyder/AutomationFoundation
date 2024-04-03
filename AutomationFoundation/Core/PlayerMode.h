#pragma once

UENUM(BlueprintType)
enum class EPlayerMode : uint8
{
	Default UMETA(DisplayName = "Default Gameplay Mode"),
	Build UMETA(DisplayName = "Build Mode")
};
