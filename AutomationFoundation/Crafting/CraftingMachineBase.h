#pragma once

#include "CraftingMachineType.h"
#include "AutomationFoundation/BuildSystem/Buildables/Placeable.h"
#include "AutomationFoundation/Interaction/IInteractable.h"
#include "CraftingMachineBase.generated.h"

UCLASS()
class ACraftingMachineBase : public APlaceable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	ECraftingMachineType MachineType;

public:
	ACraftingMachineBase();
	ACraftingMachineBase(ECraftingMachineType MachineType) : MachineType(MachineType){}

	UFUNCTION()
	ECraftingMachineType GetMachineType() const;

};
