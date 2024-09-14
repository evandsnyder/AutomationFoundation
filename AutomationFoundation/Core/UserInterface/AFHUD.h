#pragma once

#include "CoreMinimal.h"
#include "AutomationFoundation/Core/PlayerMode.h"
#include "Inventory/AFInventoryInterface.h"
#include "GameFramework/HUD.h"
#include "AFHUD.generated.h"

class UAFNotificationPanel;
class UAFToolbar;
class UAFUserWidget;
class UAFInventory;

UCLASS()
class AAFHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFInventoryInterface> InventoryClass;
	UPROPERTY()
	UAFInventoryInterface* InventoryWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFInventory> ToolbarClass;
	UPROPERTY()
	UAFInventory* ToolbarWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFNotificationPanel> InventoryNotificationPanelClass;
	UPROPERTY()
	UAFNotificationPanel* InventoryNotificationPanel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFUserWidget> BuildToolbarClass;
	UPROPERTY()
	UAFUserWidget* BuildToolbar;

	UPROPERTY()
	TArray<UAFUserWidget*> AllGameWidgets;

	UPROPERTY()
	UAFUserWidget* FocusTarget;

public:
	UFUNCTION()
	void OnPlayerModeChanged(EPlayerMode NewMode);
	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ToggleCharacterInventory();

	UFUNCTION(BlueprintCallable)
	void ToggleCraftingMachine();

	UFUNCTION(BlueprintCallable)
	void ToggleExtractionMachine();

	UFUNCTION(BlueprintCallable)
	void ToggleStorage();

	UFUNCTION(BlueprintCallable)
	void EnterBuildMode();

	UFUNCTION(BlueprintCallable)
	void ExitBuildMode();

private:
	UAFUserWidget* InstantiateWidget(const TSubclassOf<UAFUserWidget> WidgetClass) const;

	UFUNCTION()
	void ToggleWidget(UAFUserWidget* NewFocusTarget);

	UFUNCTION()
	void FocusGame() const;

	UFUNCTION()
	void FocusWidget() const;
};
