#include "CraftingMachineBase.h"

ACraftingMachineBase::ACraftingMachineBase()
{
	MachineType = ECraftingMachineType::Unknown;
}

void ACraftingMachineBase::OnInteract()
{
}

FText ACraftingMachineBase::GetInteractionText()
{
	return FText::FromString("Open Crafting Machine Base");
}

ECraftingMachineType ACraftingMachineBase::GetMachineType() const
{
	return MachineType;
}
