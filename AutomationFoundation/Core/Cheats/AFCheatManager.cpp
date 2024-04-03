#include "AFCheatManager.h"

#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "AutomationFoundation/Core/AutomationFoundationGameMode.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "AutomationFoundation/Inventory/InventoryItemInstance.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogCheatManager)

void UAFCheatManager::AddItem(const FName& ItemID, int32 Count) const
{
	if (Count < 1) return;
	AAutomationFoundationGameMode* GameMode = Cast<AAutomationFoundationGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		FItemSpecificationDataTableReference ItemSpecRef;
		if (!GameMode->FindItem(ItemID, ItemSpecRef))
		{
			LOG_WARNING(LogCheatManager, "Requested Item could not be found");
			return;
		}

		AAutomationFoundationCharacter* Character = nullptr;
		if (const APlayerController* Controller = GetPlayerController())
		{
			Character = Cast<AAutomationFoundationCharacter>(Controller->GetCharacter());
		}

		if (!Character)
		{
			LOG_WARNING(LogCheatManager, "Unable to Find Correct Character class");
			return;
		}

		while (Count > 0)
		{
			UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>();
			ItemInstance->ItemSpecification = ItemSpecRef.ItemSpecificationRef.LoadSynchronous();
			// Need to add X stacks to the player inventory as long the COUNT is greater than the max stack...
			ItemInstance->CurrentStackSize = FMath::Min(Count, ItemInstance->ItemSpecification->MaxStackSize);
			Count -= ItemInstance->CurrentStackSize;
			if (!Character->TryAddItemToInventory(ItemInstance))
			{
				// Can't add anymore items to the inventory
				// TODO: Start spawning extra items on the ground in front of player?
				return;
			}
		}
	}
}
