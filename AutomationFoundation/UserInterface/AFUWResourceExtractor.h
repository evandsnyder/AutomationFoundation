#pragma once

#include "AutomationFoundation/Core/UserInterface/AFUserWidget.h"
#include "AFUWResourceExtractor.generated.h"

class UAFInventorySlot;
class UWrapBox;
class UProgressBar;
class UInventoryItemInstance;
class AResourceExtractor;

UCLASS()
class UAFUWResourceExtractor : public UAFUserWidget
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<AResourceExtractor> OwningMachine; // Handy reference pointer...
	float CraftingTime;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFInventorySlot> InventorySlotWidget;

	UPROPERTY(meta=(BindWidget))
	UWrapBox* OutputIngredientGrid;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* CraftingProgressBar;

public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	virtual void WidgetActivated() override;
	UFUNCTION()
	virtual void WidgetDeactivated() override;

	UFUNCTION()
	void OnOutputItemUpdated(UInventoryItemInstance* Item, int32 ItemIndex);

private:
	UFUNCTION()
	void RefreshUI();
};
