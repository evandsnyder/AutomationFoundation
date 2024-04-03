#pragma once

#include "CoreMinimal.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "ToolbarComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveSlotChanged, int32, NewActiveSlot);

UCLASS()
class UToolbarComponent : public UInventoryComponent
{
	GENERATED_BODY()

protected:

	int32 CurrentSlotIndex = 0;

public:
	UToolbarComponent();

	UPROPERTY(BlueprintAssignable)
	FOnActiveSlotChanged OnActiveSlotChanged;
	
	void SetActiveSlot(int32 NewIndex);

	UFUNCTION(BlueprintCallable)
	UInventoryItemInstance* GetItemInActiveSlot() const;

	UFUNCTION(BlueprintCallable)
	int32 GetActiveSlotIndex() const;
};
