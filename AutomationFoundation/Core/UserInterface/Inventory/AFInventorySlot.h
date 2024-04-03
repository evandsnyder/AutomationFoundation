#pragma once

#include "AutomationFoundation/Core/UserInterface/AFUserWidget.h"
#include "Input/DragAndDrop.h"
#include "AFInventorySlot.generated.h"

class UInventoryComponent;
class USizeBox;
class UAFTextBlock;
class UAFButton;
class UAFImage;
class UInventoryItemInstance;

DECLARE_LOG_CATEGORY_EXTERN(LogInventorySlot, Log, All)

UCLASS()
class UAFInventorySlot : public UAFUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UAFImage* Icon;

	UPROPERTY(meta=(BindWidget))
	UAFTextBlock* StackSizeLabel;

	UPROPERTY(meta=(BindWidget))
	USizeBox* StackCountBox;

	UPROPERTY()
	UInventoryItemInstance* ItemInstance;
	UPROPERTY()
	int32 ItemIndex;

	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> OwningInventory;
	
public:

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void ConfigureSlot(UInventoryItemInstance* NewItemInstance, int32 NewItemIndex);

	UFUNCTION()
	void SetItemSlot(int32 NewSlotIndex);

	UFUNCTION()
	void SetOwningInventory(UInventoryComponent* NewOwner);

	UFUNCTION()
	void ApplyFilter(const FName& ItemID);

protected:
	// virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// highlights?
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
