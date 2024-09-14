#include "AFUWResourceExtractor.h"

#include "AutomationFoundation/Core/UserInterface/Inventory/AFInventorySlot.h"
#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Interaction/InteractComponent.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "AutomationFoundation/Mining/ResourceExtractor.h"
#include "Components/ProgressBar.h"
#include "Components/WrapBox.h"

void UAFUWResourceExtractor::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwningMachine.IsValid() && CraftingProgressBar)
	{
		CraftingProgressBar->SetPercent(OwningMachine->GetCraftingProgress());
	}
}

void UAFUWResourceExtractor::WidgetActivated_Implementation()
{
	const AAutomationFoundationCharacter* Character = Cast<AAutomationFoundationCharacter>(GetOwningPlayerPawn());

	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owning player was not an AFCharacter"));
		return;
	}

	UInteractComponent* InteractComponent = Character->GetComponentByClass<UInteractComponent>();

	if (!IsValid(InteractComponent))
	{
		LOG_WARNING(LogTemp, "Could not get the interact component from the player");
		return;
	}

	OwningMachine = Cast<AResourceExtractor>(InteractComponent->GetInteractTarget());
	if (!OwningMachine.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not bind to Resource Extractor/Mining Drill. Invalid Actor pointer"));
		return;
	}

	RefreshUI();
}

void UAFUWResourceExtractor::WidgetDeactivated_Implementation()
{
	Super::WidgetDeactivated();

	if (OwningMachine.IsValid())
	{
		UInventoryComponent* Inventory = IItemProvider::Execute_GetProviderInventory(OwningMachine.Get());
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

	UInventoryComponent* Inventory = IItemProvider::Execute_GetProviderInventory(OwningMachine.Get());
	if (Inventory)
	{
		Inventory->OnItemChanged.RemoveAll(this);
		Inventory->OnItemChanged.AddDynamic(this, &UAFUWResourceExtractor::OnOutputItemUpdated);
	}

	// TODO: Update if there are any input items...

	// OwningMachine->CurrentRecipe->OutputItems{
	// }

	// TODO: This works because we assume we are only mining one resource...
	UAFInventorySlot* NewOutputWidget = CreateWidget<UAFInventorySlot>(this, InventorySlotWidget);
	NewOutputWidget->SetItemSlot(0);
	NewOutputWidget->SetOwningInventory(Inventory);
	OutputIngredientGrid->AddChildToWrapBox(NewOutputWidget);
}
