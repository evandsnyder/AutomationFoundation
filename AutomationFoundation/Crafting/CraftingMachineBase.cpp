#include "CraftingMachineBase.h"

ACraftingMachineBase::ACraftingMachineBase()
{
	MachineType = ECraftingMachineType::Unknown;
}

ECraftingMachineType ACraftingMachineBase::GetMachineType() const
{
	return MachineType;
}
