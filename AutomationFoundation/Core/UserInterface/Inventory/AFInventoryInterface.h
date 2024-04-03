#pragma once

#include "CoreMinimal.h"
#include "AutomationFoundation/Core/UserInterface/AFUserWidget.h"
#include "AutomationFoundation/Inventory/InventoryItemInstance.h"
#include "AFInventoryInterface.generated.h"

class UAFInventory;
class AAutomationFoundationCharacter;
class UWrapBox;
class UWidgetSwitcher;
class UInventoryComponent;

UCLASS()
class UAFInventoryInterface : public UAFUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFUserWidget> InventoryCraftingClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFUserWidget> CraftingMachineClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFUserWidget> ExtractorInterfaceClass;

	// MUST have the Inventory and the Switcher.
	// We will dynamically add the Crafting Machine and the InventoryCrafting Interface
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* CraftingInterfaceSwitcher;

	UPROPERTY(meta=(BindWidget))
	UAFInventory* PlayerInventory;

	UPROPERTY()
	UAFUserWidget* InventoryCraftingWidget;

	UPROPERTY()
	UAFUserWidget* CraftingMachineWidget;

	UPROPERTY()
	UAFUserWidget* ExtractionMachineWidget;

	UPROPERTY()
	AAutomationFoundationCharacter* OwningCharacter;
	UPROPERTY()
	UInventoryComponent* InventoryComponent;

protected:
	virtual void NativeConstruct() override;

public:
	void OnOpenCraftingMachine();
	void OnOpenPlayerInventory();
	void OnOpenExtractionMachine();

	virtual void WidgetActivated() override;
};