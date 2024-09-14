#include "AFInventory.h"

#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "AutomationFoundation/Core/UserInterface/Inventory/AFInventorySlot.h"
#include "Components/WrapBox.h"

DEFINE_LOG_CATEGORY(LogInventoryWidget);

void UAFInventory::Configure(UInventoryComponent* NewInventoryComponent)
{
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->OnItemChanged.RemoveDynamic(this, &UAFInventory::OnInventoryItemChanged);
	}

	InventoryComponent = NewInventoryComponent;
	if (!IsValid(InventoryComponent))
	{
		// Not a valid inventory...
		LOG_ERROR(LogInventoryWidget, "Provided Inventory Is not Valid");
		return;
	}

	InventoryComponent->OnItemChanged.AddDynamic(this, &UAFInventory::OnInventoryItemChanged);

	Refresh();
}

void UAFInventory::Refresh()
{
	InventoryGrid->ClearChildren();
	// Need to rebuild and re-populate all of our inventory pieces...
	if (!IsValid(InventoryComponent))
	{
		LOG_WARNING(LogTemp, "Inventory Is Invalid");
		return;
	}
	for (int32 i = 0; i < InventoryComponent->GetInventorySize(); i++)
	{
		UAFInventorySlot* InventorySlot = CreateWidget<UAFInventorySlot>(this, InventorySlotClass);
		InventorySlot->SetItemSlot(i);
		InventorySlot->ConfigureSlot(InventoryComponent->Inventory[i], i);
		InventorySlot->SetOwningInventory(InventoryComponent);
		InventoryGrid->AddChildToWrapBox(InventorySlot);
	}
}

void UAFInventory::OnInventoryItemChanged_Implementation(UInventoryItemInstance* ItemInstance, int32 ItemIndex)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Inventory Item Added"));
	}

	if (UAFInventorySlot* ItemSlot = Cast<UAFInventorySlot>(InventoryGrid->GetChildAt(ItemIndex)); IsValid(ItemSlot))
	{
		ItemSlot->ConfigureSlot(ItemInstance, ItemIndex);
	}
}

UInventoryComponent* UAFInventory::GetInventoryComponent() const
{
	return InventoryComponent;
}
