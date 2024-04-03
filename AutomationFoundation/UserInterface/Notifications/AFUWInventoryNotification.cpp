#include "AFUWInventoryNotification.h"

#include "AutomationFoundation/Core/UserInterface/AFImage.h"
#include "AutomationFoundation/Core/UserInterface/AFTextBlock.h"
#include "AutomationFoundation/Inventory/InventoryItemInstance.h"

void UAFUWInventoryNotification::Configure(const UInventoryItemInstance* ItemInstance)
{
	if (IsValid(ItemInstance))
	{
		Icon->SetBrushFromSoftTexture(ItemInstance->ItemSpecification->Icon);

		const FText FormatPattern = FText::FromString(TEXT("{0} x{1}"));
		DisplayText->SetText(FText::Format(FormatPattern, ItemInstance->ItemSpecification->DisplayName, ItemInstance->CurrentStackSize));
	}
}
