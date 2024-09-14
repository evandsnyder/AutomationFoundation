#pragma once

#include "Placeable.h"
#include "AutomationFoundation/Crafting/RecipeSpecification.h"
#include "AutomationFoundation/Interaction/ICanInteract.h"
#include "AutomationFoundation/Interaction/IInteract.h"
#include "AutomationFoundation/Inventory/IItemAcceptor.h"
#include "AutomationFoundation/Inventory/IItemProvider.h"
#include "Machine.generated.h"

class UItemSpecification;
class AConveyorItem;
class UInventoryItemInstance;
class AAttachPoint;

UCLASS()
class AMachine : public APlaceable, public IInteract, public ICanInteract, public IItemAcceptor, public IItemProvider
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	TMap<AAttachPoint*, UInventoryComponent*> Outputs;
	UPROPERTY(BlueprintReadWrite)
	TMap<AAttachPoint*, UInventoryComponent*> Inputs;

	float ItemSpawnInterval = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AConveyorItem> ConveyorItemClass;

	UPROPERTY(EditAnywhere)
	bool bIsPlaced = false;

	UPROPERTY(EditAnywhere)
	URecipeSpecification* CurrentRecipe;

	UPROPERTY(BlueprintReadWrite)
	bool bCanOutputNextItem = true;

	UPROPERTY()
	FTimerHandle CraftingTimer;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCrafting = false;

public:
	AMachine();

	UFUNCTION(BlueprintCallable)
	void ConnectAttachPointAsInput(AAttachPoint* AttachPoint);
	UFUNCTION(BlueprintCallable)
	void ConnectAttachPointAsOutput(AAttachPoint* AttachPoint);

	UFUNCTION(BlueprintCallable)
	bool OutputFromAttachPoint(AAttachPoint* OutputAttachPoint, UItemSpecification* ItemSpecification);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool CanOutput();
	UFUNCTION(BlueprintNativeEvent)
	void StartOutputting();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryAcceptInput(AAttachPoint* ProvidingAttachPoint, AConveyorItem* IncomingItem);

	virtual FString GetInteractText() override;
	virtual bool CanInteract() override;


	virtual bool PlacePreview_Implementation() override;

	/**
	 * Changes the current recipe and refreshes the input/output inventories
	 * @param NewRecipe 
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateRecipe(URecipeSpecification* NewRecipe);

	/**
	 * 
	 */
	UFUNCTION(BlueprintCallable)
	void StartCrafting();

	/**
	 * Event that occurs when a crafting cycle has completed
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnCraftingComplete();

	/**
	 * Terminates the current crafting cycle. This may occur because a recipe changes or because items are removed from the input inventory 
	 */
	UFUNCTION()
	void CancelCraftingProcess();
	/**
	 * Gets the progress of the currently in-use crafting recipe
	 * @return The crafting progress as a percent between 0 and 1
	 */
	UFUNCTION(BlueprintCallable)
	float GetCraftingProgress() const;

protected:
	virtual void BeginPlay() override;

private:
	bool CanCraft();

	bool InputIngredientsAreSatisfied();
	bool IsOutputFull();
};
