#pragma once

#include "CraftingMachineType.h"
#include "AutomationFoundation/Interaction/IInteractable.h"
#include "CraftingMachineBase.generated.h"

UCLASS(Abstract)
class ACraftingMachineBase : public AActor, public IInteractable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	ECraftingMachineType MachineType;

public:
	ACraftingMachineBase();
	ACraftingMachineBase(ECraftingMachineType MachineType) : MachineType(MachineType){}

	virtual void OnInteract(AActor* InteractInstigator) override;
	virtual FText GetInteractionText() override;

	UFUNCTION()
	ECraftingMachineType GetMachineType() const;

};
