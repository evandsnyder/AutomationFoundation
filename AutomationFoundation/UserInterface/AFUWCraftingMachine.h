#pragma once

#include "AutomationFoundation/Core/UserInterface/AFUserWidget.h"
#include "AutomationFoundation/Crafting/RecipeSpecification.h"
#include "AFUWCraftingMachine.generated.h"

class UAFInventorySlot;
class UInventoryItemInstance;
class UAFSelectRecipeWidget;
class UProgressBar;
class UAFTextBlock;
class UAFButton;
class ACraftingMachine;
class UWrapBox;

UCLASS(Abstract)
class UAFUWCraftingMachine : public UAFUserWidget
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<ACraftingMachine> OwningMachine; // Handy reference pointer...
	float CraftingTime;

	UPROPERTY(meta=(BindWidget))
	UAFButton* ChangeRecipeButton;

	UPROPERTY(meta=(BindWidget))
	UAFTextBlock* RecipeNameLabel;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* CraftingProgressBar;
	UPROPERTY(meta=(BindWidget))
	UWrapBox* InputIngredientGrid;
	UPROPERTY(meta=(BindWidget))
	UWrapBox* OutputIngredientGrid;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFUserWidget> SelectRecipeClass;
	UPROPERTY()
	UAFSelectRecipeWidget* SelectRecipeWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFInventorySlot> InventorySlotWidget;

protected:
	virtual void NativeConstruct() override;

public:
	virtual void WidgetActivated_Implementation() override;
	virtual void WidgetDeactivated_Implementation() override;

	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnChangeRecipeButtonClicked();

	UFUNCTION()
	void OnCraftingRecipeChanged(URecipeSpecification* NewSpecification);

	UFUNCTION()
	void RefreshUI();

	UFUNCTION()
	void OnInputItemChanged(UInventoryItemInstance* Item, int32 ItemIndex);
	UFUNCTION()
	void OnOutputItemChanged(UInventoryItemInstance* Item, int32 ItemIndex);
};
