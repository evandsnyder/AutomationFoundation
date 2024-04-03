#include "AFInventorySlot.h"

#include "AFInventory.h"
#include "AutomationFoundation/Core/AutomationFoundationGameMode.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Core/UserInterface/AFImage.h"
#include "AutomationFoundation/Core/UserInterface/AFTextBlock.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "AutomationFoundation/Inventory/InventoryItemInstance.h"
#include "AutomationFoundation/UserInterface/InventoryDragDropOperation.h"
#include "Components/SizeBox.h"

DEFINE_LOG_CATEGORY(LogInventorySlot);

void UAFInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAFInventorySlot::ConfigureSlot(UInventoryItemInstance* NewItemInstance, int32 NewItemIndex)
{
	ItemInstance = NewItemInstance;
	// ItemIndex = NewItemIndex;

	if (!IsValid(ItemInstance))
	{
		Icon->SetVisibility(ESlateVisibility::Hidden);
		StackCountBox->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	Icon->SetBrushFromSoftTexture(ItemInstance->ItemSpecification->Icon);
	Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	StackSizeLabel->SetText(FText::AsNumber(ItemInstance->CurrentStackSize));
	StackCountBox->SetVisibility(ItemInstance->CurrentStackSize > 1 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UAFInventorySlot::SetItemSlot(int32 NewSlotIndex)
{
	ItemIndex = NewSlotIndex;
}

void UAFInventorySlot::SetOwningInventory(UInventoryComponent* NewOwner)
{
	OwningInventory = NewOwner;
}

void UAFInventorySlot::ApplyFilter(const FName& ItemID)
{
	// This set the icon and apply a mask to make it all black if Item is null or stack size == 0
	if (AAutomationFoundationGameMode* GameMode = Cast<AAutomationFoundationGameMode>(GetWorld()->GetAuthGameMode()))
	{
		FItemSpecificationDataTableReference ItemSpec;
		if (GameMode->FindItem(ItemID, ItemSpec))
		{
			Icon->SetBrushFromSoftTexture(ItemSpec.ItemSpecificationRef.LoadSynchronous()->Icon);
		}
	}
}

FReply UAFInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	LOG_INFO(LogInventorySlot, "Clicking...");
	if (!IsValid(ItemInstance))
	{
		// Can't really D&D an empty tile
		return FReply::Unhandled();
	}

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		const TSharedPtr<SWidget> SlateWidgetDetectingDrag = GetCachedWidget();
		if (SlateWidgetDetectingDrag.IsValid())
		{
			return FReply::Handled().DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), EKeys::LeftMouseButton);
		}
	}

	return FReply::Unhandled();
}

bool UAFInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	LOG_INFO(LogInventorySlot, "Attempting Drop");

	if (const UInventoryDragDropOperation* Operation = Cast<UInventoryDragDropOperation>(InOperation))
	{
		LOG_INFO(LogInventorySlot, "InventoryItem Dropped");

		if (OwningInventory.IsValid())
		{
			OwningInventory->Transfer(Operation->ItemIndex, Operation->SourceInventory, ItemIndex);
			return true;
		}
		LOG_WARNING(LogTemp, "Failed to get Destination Inventory Component");
	}

	return false;
}

void UAFInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	LOG(LogInventorySlot, Display, FColor::Green, "Dragging Started!");
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UInventoryDragDropOperation* DragDropOperation = NewObject<UInventoryDragDropOperation>();

	if (OwningInventory.IsValid())
	{
		DragDropOperation->SourceInventory = OwningInventory.Get();
		DragDropOperation->ItemIndex = ItemIndex;
		DragDropOperation->DefaultDragVisual = this;
		DragDropOperation->Pivot = EDragPivot::MouseDown;
		OutOperation = DragDropOperation;
	}
}

void UAFInventorySlot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
}

void UAFInventorySlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}
