#pragma once

#include "InventoryItemInstance.h"
#include "InventoryComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryComponent, Log, All)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChangedDelegate, UInventoryItemInstance*, NewItem, int32, ItemIndex);

UCLASS()
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 InventorySize = 30;

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UInventoryItemInstance>> Inventory;

	UPROPERTY(BlueprintAssignable)
	FOnItemChangedDelegate OnItemChanged;

	UPROPERTY()
	TArray<FName> InventoryFilters;

public:
	UInventoryComponent();

	UFUNCTION()
	bool TryAddItemToInventory(UInventoryItemInstance* NewItem);
	UFUNCTION()
	bool RemoveItemFromInventory(UInventoryItemInstance* Item);

	bool Transfer(int32 SourceIndex, UInventoryComponent* SourceInventory, int32 DestinationIndex);
	UFUNCTION()
	const UInventoryItemInstance* FindFirstInstanceOrNull(const FName& ItemID) const;

	UFUNCTION()
	void AddInventoryFilter(const FName& ItemID, int32 InventoryIndex);
	UFUNCTION()
	void RemoveInventoryFilter(int32 InventoryIndex);

	UFUNCTION(BlueprintCallable)
	void SetInventorySize(int32 NewInventorySize);

	UFUNCTION()
	void ResetFilters();

	UFUNCTION(BlueprintCallable)
	int32 GetInventorySize() const;

private:
	bool HasAvailableSpace() const;

	// will return true if an item can be placed at the provided slot, false otherwise
	bool CheckFilter(int32 Index, const FName& ItemID) const;
};
