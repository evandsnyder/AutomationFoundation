#pragma once

#include "InventoryItemInstance.h"
#include "AutomationFoundation/Crafting/RecipeSpecification.h"
#include "InventoryComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryComponent, Log, All)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChangedDelegate, UInventoryItemInstance*, NewItem, int32, ItemIndex);

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventorySize = 30;

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UInventoryItemInstance>> Inventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnItemChangedDelegate OnItemChanged;

	UPROPERTY()
	TArray<FName> InventoryFilters;

	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	bool TryAddItemToInventory(UInventoryItemInstance* NewItem);
	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromInventory(UInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable)
	bool Transfer(int32 SourceIndex, UInventoryComponent* SourceInventory, int32 DestinationIndex);
	UFUNCTION(BlueprintCallable)
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

	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const;

	UFUNCTION(BlueprintCallable)
	UInventoryItemInstance* FindFirstItem(int32& Index) const;

	UFUNCTION(BlueprintCallable)
	void ConfigureInventoryFromRecipe(URecipeSpecification* NewRecipe, bool bIsInput = true)
	{
		ResetFilters();
		TMap<FName, int32>& Map = bIsInput ? NewRecipe->InputItems : NewRecipe->OutputItems;

		// TODO: Add method for emptying the inventory either into a player inventory or into the world
		SetInventorySize(Map.Num());

		int32 i = 0;
		for (const TPair<FName, int32>& Entry : Map)
		{
			AddInventoryFilter(Entry.Key, i);
			i++;
		}
	}

private:
	bool HasAvailableSpace() const;

	// will return true if an item can be placed at the provided slot, false otherwise
	bool CheckFilter(int32 Index, const FName& ItemID) const;
};
