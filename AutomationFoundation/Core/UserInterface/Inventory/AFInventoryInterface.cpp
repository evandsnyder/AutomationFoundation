#include "AFInventoryInterface.h"

#include "AFInventory.h"
#include "AFInventorySlot.h"
#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WrapBox.h"

void UAFInventoryInterface::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(IsValid(CraftingInterfaceSwitcher));
	ensure(IsValid(PlayerInventory));

	if (!IsValid(InventoryCraftingWidget))
	{
		InventoryCraftingWidget = CreateWidget<UAFUserWidget>(this, InventoryCraftingClass);
	}

	if (!IsValid(CraftingMachineWidget))
	{
		CraftingMachineWidget = CreateWidget<UAFUserWidget>(this, CraftingMachineClass);
	}

	CraftingInterfaceSwitcher->AddChild(InventoryCraftingWidget);
	CraftingInterfaceSwitcher->AddChild(CraftingMachineWidget);
	CraftingInterfaceSwitcher->SetActiveWidget(InventoryCraftingWidget);

	OwningCharacter = Cast<AAutomationFoundationCharacter>(GetOwningPlayerPawn());

	if (!OwningCharacter)
	{
		// We've got a problem...
		UE_LOG(LogTemp, Warning, TEXT("Could not get the appropriate character"));
		return;
	}

	InventoryComponent = Cast<UInventoryComponent>(OwningCharacter->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (IsValid(InventoryComponent))
	{
		PlayerInventory->Configure(InventoryComponent);
	}
}

void UAFInventoryInterface::OnOpenCraftingMachine()
{
	if (!IsValid(CraftingMachineWidget))
	{
		CraftingMachineWidget = CreateWidget<UAFUserWidget>(this, CraftingMachineClass);
		CraftingInterfaceSwitcher->AddChild(CraftingMachineWidget);
	}

	CraftingInterfaceSwitcher->SetActiveWidget(CraftingMachineWidget);
	CraftingMachineWidget->WidgetActivated();
}

void UAFInventoryInterface::OnOpenPlayerInventory()
{
	if (!IsValid(InventoryCraftingWidget))
	{
		InventoryCraftingWidget = CreateWidget<UAFUserWidget>(this, InventoryCraftingClass);
		CraftingInterfaceSwitcher->AddChild(InventoryCraftingWidget);
	}

	CraftingInterfaceSwitcher->SetActiveWidget(InventoryCraftingWidget);
	InventoryCraftingWidget->WidgetActivated();
}

void UAFInventoryInterface::OnOpenExtractionMachine()
{
	if(!IsValid(ExtractionMachineWidget))
	{
		ExtractionMachineWidget = CreateWidget<UAFUserWidget>(this, ExtractorInterfaceClass);
		CraftingInterfaceSwitcher->AddChild(ExtractionMachineWidget);
	}

	CraftingInterfaceSwitcher->SetActiveWidget(ExtractionMachineWidget);
	ExtractionMachineWidget->WidgetActivated();
}

void UAFInventoryInterface::WidgetActivated()
{
	Super::WidgetActivated();
	PlayerInventory->WidgetActivated();
}
