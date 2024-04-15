#include "InventoryItemInstance.h"

UInventoryItemInstance::UInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UStaticMesh* UInventoryItemInstance::GetDisplayMesh() const
{
	return ItemSpecification->DisplayMesh.LoadSynchronous();
}

bool UInventoryItemInstance::HasClassificationFlag(EItemClassification ItemClassificationFlag) const
{
	return ItemSpecification && ItemSpecification->ItemClassificationFlags & static_cast<uint8>(ItemClassificationFlag);
}

void UInventoryItemInstance::ModifyCurrentStackSize(int32 Value)
{
	CurrentStackSize += Value;
}
