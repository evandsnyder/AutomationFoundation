#include "InventoryComponent.h"

#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent)

UInventoryComponent::UInventoryComponent()
{
	Inventory = TArray<TObjectPtr<UInventoryItemInstance>>();
	Inventory.Init(nullptr, InventorySize);

	InventoryFilters = TArray<FName>();
	InventoryFilters.Init("", InventorySize);
}

/**
 * Attempts to add an item to the inventory
 * @param NewItem The Item to Add to the inventory 
 * @return Success indicator to determine if Item was added to the inventory
 */
bool UInventoryComponent::TryAddItemToInventory(UInventoryItemInstance* NewItem)
{
	// First, find any instances in the inventory that are the same item, but not full stacks
	for (int i = 0; i < InventorySize; i++)
	{
		UInventoryItemInstance* Item = Inventory[i];

		if (!IsValid(Item)) continue; // For now, skip if this is a nullptr
		if (Item->ItemSpecification->ItemID == NewItem->ItemSpecification->ItemID &&
			Item->CurrentStackSize < Item->ItemSpecification->MaxStackSize
		)
		{
			// Conduct a partial move...
			Item->CurrentStackSize += NewItem->CurrentStackSize;
			NewItem->CurrentStackSize = 0;
			if (Item->CurrentStackSize > Item->ItemSpecification->MaxStackSize)
			{
				NewItem->CurrentStackSize = Item->CurrentStackSize - Item->ItemSpecification->MaxStackSize;
				Item->CurrentStackSize = Item->ItemSpecification->MaxStackSize;
			}

			OnItemChanged.Broadcast(Item, i);
			if (NewItem->CurrentStackSize == 0)
			{
				// we've consume the entire input. we're done here.
				return true;
			}
		}
	}

	// Okay, we've added everything we can to the existing stacks, now we have to add it to the first open stack
	for (int i = 0; i < InventorySize; i++)
	{
		if (Inventory[i] == nullptr && CheckFilter(i, NewItem->ItemSpecification->ItemID))
		{
			Inventory[i] = NewItem;
			OnItemChanged.Broadcast(NewItem, i);
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::RemoveItemFromInventory(UInventoryItemInstance* Item)
{
	const int32 ItemIndex = Inventory.Find(Item);
	// if the item isn't found, there is nothing to drop...
	const bool bFound = ItemIndex != INDEX_NONE;
	if (bFound)
	{
		Inventory[ItemIndex] = nullptr;
		OnItemChanged.Broadcast(nullptr, ItemIndex);
		// TODO: Spawn Actor in the world if the stack count is greater than 0
		if (Item->CurrentStackSize > 0)
		{
			// GetWorld()->SpawnActor()
		}
	}

	return bFound;
}


// Destination has transfer invoked....
bool UInventoryComponent::Transfer(int32 SourceIndex, UInventoryComponent* SourceInventory, int32 DestinationIndex)
{
	// TODO: Check for Filters

	if (SourceInventory == this && SourceIndex == DestinationIndex)
	{
		// there isn't anything to really do. We're moving the item to the exact same position..
		return true;
	}

	// Validate Source Index and Destination Index..
	if (DestinationIndex < 0 || DestinationIndex > InventorySize)
	{
		// Invalid destination Index
		LOG_WARNING(LogInventoryComponent, "Destination Index is Invalid")
		return false;
	}

	if (!IsValid(SourceInventory))
	{
		// Coming from Nowhere
		LOG_WARNING(LogInventoryComponent, "Source Inventory Is Invalid")
		return false;
	}

	if (SourceIndex < 0 || SourceIndex > SourceInventory->InventorySize)
	{
		// Source Index is invalid...
		LOG_WARNING(LogInventoryComponent, "Source Index is Invalid")
		return false;
	}

	// Several things can happen
	// 1. Destination spot is nullptr
	// 2. Items are different at each spot
	// 4. Item stack is full
	// 5. Items are same and amount to less than a full stack
	// 6. Items are same but overflow stack size

	UInventoryItemInstance* SourceInstance = SourceInventory->Inventory[SourceIndex];
	UInventoryItemInstance* DestinationInstance = Inventory[DestinationIndex];
	UItemSpecification* ItemSpecification = SourceInstance->ItemSpecification;

	// If destination is nullptr, does it have a filter??
	// 1.
	if (!IsValid(DestinationInstance) && CheckFilter(DestinationIndex, ItemSpecification->ItemID))
	{
		Inventory[DestinationIndex] = SourceInstance;

		// Warning: this will ignore filters in the source inventory
		SourceInventory->Inventory[SourceIndex] = DestinationInstance;

		OnItemChanged.Broadcast(SourceInstance, DestinationIndex);
		SourceInventory->OnItemChanged.Broadcast(DestinationInstance, SourceIndex);

		return true;
	}

	//  2. && 4.
	if (SourceInstance->ItemSpecification->ItemID != ItemSpecification->ItemID ||
		DestinationInstance->CurrentStackSize == ItemSpecification->MaxStackSize)
	{
		Inventory[DestinationIndex] = SourceInstance;
		SourceInventory->Inventory[SourceIndex] = DestinationInstance;

		OnItemChanged.Broadcast(SourceInstance, DestinationIndex);
		SourceInventory->OnItemChanged.Broadcast(DestinationInstance, SourceIndex);
		return true;
	}

	// If we are still here, then the source and destination instance IDs are the same.
	// 4.
	if (SourceInstance->CurrentStackSize + DestinationInstance->CurrentStackSize <= ItemSpecification->MaxStackSize)
	{
		DestinationInstance->CurrentStackSize += SourceInstance->CurrentStackSize;
		SourceInventory->Inventory[SourceIndex] = nullptr;

		OnItemChanged.Broadcast(DestinationInstance, DestinationIndex);
		SourceInventory->OnItemChanged.Broadcast(nullptr, SourceIndex);

		return true;
	}

	// 5. we Can only do a partial move...
	// Update the source first...
	SourceInstance->CurrentStackSize -= ItemSpecification->MaxStackSize - DestinationInstance->CurrentStackSize;
	DestinationInstance->CurrentStackSize = ItemSpecification->MaxStackSize;

	SourceInventory->OnItemChanged.Broadcast(SourceInstance, SourceIndex);
	OnItemChanged.Broadcast(DestinationInstance, DestinationIndex);

	return true;
}

const UInventoryItemInstance* UInventoryComponent::FindFirstInstanceOrNull(const FName& ItemID) const
{
	for (const UInventoryItemInstance* Item : Inventory)
	{
		if (Item->ItemSpecification->ItemID == ItemID) return Item;
	}

	return nullptr;
}

/**
 * Will take an ItemID and slot and ensure that the inventory only permits that Item in it
 * @param ItemID Item to Whitelist in slot
 * @param InventoryIndex Which inventory slot to apply the whitelist to
 */
void UInventoryComponent::AddInventoryFilter(const FName& ItemID, int32 InventoryIndex)
{
	if (InventoryIndex < 0 || InventoryIndex >= InventorySize) return;
	InventoryFilters[InventoryIndex] = ItemID;
}

void UInventoryComponent::RemoveInventoryFilter(int32 InventoryIndex)
{
	InventoryFilters[InventoryIndex] = "";
}

void UInventoryComponent::SetInventorySize(int32 NewInventorySize)
{
	// TODO: Check if inventory is shrinking? If so, are we going to lose items?
	// Do we need to throw the other items into the world?
	InventorySize = NewInventorySize;
	Inventory.SetNum(NewInventorySize);
	InventoryFilters.SetNum(NewInventorySize);
}

void UInventoryComponent::ResetFilters()
{
	InventoryFilters.Reset();
}

int32 UInventoryComponent::GetInventorySize() const
{
	return InventorySize;
}

bool UInventoryComponent::IsEmpty() const
{
	return Inventory.IsEmpty();
}

UInventoryItemInstance* UInventoryComponent::FindFirstItem(int32& Index) const
{
	for (int i = 0; i < InventorySize; i++)
	{
		if (IsValid(Inventory[i]))
		{
			Index = i;
			return Inventory[Index];
		}
	}

	Index = INDEX_NONE;
	return nullptr;
}

bool UInventoryComponent::HasAvailableSpace() const
{
	return INDEX_NONE != Inventory.Find(nullptr);
}

bool UInventoryComponent::CheckFilter(int32 Index, const FName& ItemID) const
{
	return InventoryFilters[Index] == ItemID || InventoryFilters[Index] == "";
}
