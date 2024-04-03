#pragma once

#include "../AFUserWidget.h"
#include "AFInventory.generated.h"

class UInventoryItemInstance;
class UAFInventorySlot;
class UInventoryComponent;
class UWrapBox;

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryWidget, Log, All);

UCLASS()
class UAFInventory : public UAFUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UWrapBox* InventoryGrid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAFInventorySlot> InventorySlotClass;

	UPROPERTY()
	UInventoryComponent* InventoryComponent;

public:
	UFUNCTION(BlueprintCallable)
	void Configure(UInventoryComponent* NewInventoryComponent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInventoryItemChanged(UInventoryItemInstance* ItemInstance, int32 ItemIndex);


	UInventoryComponent* GetInventoryComponent() const;

	virtual void WidgetActivated() override;
	void Refresh();
};
