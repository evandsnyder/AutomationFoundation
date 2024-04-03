#include "AFUWResourceExtractor.h"

#include "AutomationFoundation/Core/UserInterface/Inventory/AFInventorySlot.h"
#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "AutomationFoundation/Mining/ResourceExtractor.h"
#include "Components/ProgressBar.h"
#include "Components/WrapBox.h"

void UAFUWResourceExtractor::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(OwningMachine.IsValid() && CraftingProgressBar)
	{
		CraftingProgressBar->SetPercent(OwningMachine->GetCraftingProgress());
	}
}

void UAFUWResourceExtractor::WidgetActivated()
{
	Super::WidgetActivated();

	const AAutomationFoundationCharacter* Character = Cast<AAutomationFoundationCharacter>(GetOwningPlayerPawn());

	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owning player was not an AFCharacter"));
		return;
	}

	OwningMachine = Cast<AResourceExtractor>(Character->CurrentInteractableActor);
	if (!OwningMachine.IsValid())
	{
		return;
	}

	RefreshUI();
}

void UAFUWResourceExtractor::WidgetDeactivated()
{
	Super::WidgetDeactivated();

	if (OwningMachine.IsValid())
	{
		UInventoryComponent* Inventory = OwningMachine->GetProviderInventory();
		if (Inventory)
		{
			Inventory->OnItemChanged.RemoveDynamic(this, &UAFUWResourceExtractor::OnOutputItemUpdated);
		}

		OwningMachine.Reset();
	}
}

void UAFUWResourceExtractor::OnOutputItemUpdated(UInventoryItemInstance* Item, int32 ItemIndex)
{
	UAFInventorySlot* ItemSlot = Cast<UAFInventorySlot>(OutputIngredientGrid->GetChildAt(ItemIndex));
	if (IsValid(ItemSlot))
	{
		ItemSlot->ConfigureSlot(Item, ItemIndex);
	}
}

void UAFUWResourceExtractor::RefreshUI()
{
	if (!OwningMachine.IsValid()) return;

	OutputIngredientGrid->ClearChildren();

	UInventoryComponent* Inventory = OwningMachine->GetProviderInventory();
	if (Inventory)
	{
		Inventory->OnItemChanged.AddDynamic(this, &UAFUWResourceExtractor::OnOutputItemUpdated);
	}

	// assume we are only mining one resource...
	UAFInventorySlot* NewOutputWidget = CreateWidget<UAFInventorySlot>(this, InventorySlotWidget);
	NewOutputWidget->SetItemSlot(0);
	NewOutputWidget->SetOwningInventory(Inventory);
	OutputIngredientGrid->AddChildToWrapBox(NewOutputWidget);
}
