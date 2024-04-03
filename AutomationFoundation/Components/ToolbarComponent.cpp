#include "ToolbarComponent.h"

#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"

UToolbarComponent::UToolbarComponent()
{
	SetInventorySize(10);
}

void UToolbarComponent::SetActiveSlot(int32 NewIndex)
{
	if (NewIndex < 0 || NewIndex > InventorySize || NewIndex == CurrentSlotIndex)
	{
		// Can't do anything..
		return;
	}

	CurrentSlotIndex = NewIndex;
	OnActiveSlotChanged.Broadcast(CurrentSlotIndex);
	LOG_INFO(LogTemp, "Updated Toolbar Index to %d", CurrentSlotIndex);
}

UInventoryItemInstance* UToolbarComponent::GetItemInActiveSlot() const
{
	return Inventory[CurrentSlotIndex];
}

int32 UToolbarComponent::GetActiveSlotIndex() const
{
	return CurrentSlotIndex;
}
