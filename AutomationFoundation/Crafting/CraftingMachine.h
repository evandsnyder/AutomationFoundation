#pragma once

#include "CoreMinimal.h"
#include "CraftingMachineBase.h"
#include "RecipeSpecification.h"
#include "AutomationFoundation/Interaction/ICanInteract.h"
#include "AutomationFoundation/Interaction/IInteract.h"
#include "AutomationFoundation/Inventory/IItemAcceptor.h"
#include "AutomationFoundation/Inventory/IItemProvider.h"
#include "AutomationFoundation/Inventory/InventoryItemInstance.h"
#include "CraftingMachine.generated.h"

UENUM(BlueprintType)
enum class EMachineStatus : uint8
{
	Unknown,
	NoRecipeSet,
	UnneededIngredient,
	InventoryFull,
	ItemConsumedComplete,
	ItemConsumedPartial
};

UCLASS(BlueprintType, Blueprintable)
class ACraftingMachine : public ACraftingMachineBase, public IItemAcceptor, public IItemProvider, public ICanInteract, public IInteract
{
	GENERATED_BODY()

	TObjectPtr<class AAutomationFoundationCharacter> PlayerCharacter;

public:
	UPROPERTY(EditAnywhere)
	URecipeSpecification* CurrentRecipe;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsCrafting;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bCanCraft;

	UPROPERTY()
	UInventoryComponent* InputInventory;
	UPROPERTY()
	UInventoryComponent* OutputInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimerHandle CraftingTimer;

	ACraftingMachine();

	virtual void BeginPlay() override;

	UFUNCTION()
	void SetCraftingRecipe(URecipeSpecification* NewRecipe);

	UFUNCTION(BlueprintNativeEvent)
	void OnCraftingComplete();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInputItemAdded(UInventoryItemInstance* NewItem, int32 ItemIndex);

	UFUNCTION()
	void OnOutputItemRemoved(UInventoryItemInstance* RemovedItem, int32 OldIndex);

	UFUNCTION()
	bool AreInputIngredientsSatisfied() const;
	UFUNCTION()
	bool IsOutputFull() const;

	UFUNCTION()
	bool CanCraft() const;

	UFUNCTION()
	float GetCraftingProgress() const;

	UFUNCTION()
	virtual UInventoryComponent* GetAcceptorInventory_Implementation() final override;

	UFUNCTION()
	virtual UInventoryComponent* GetProviderInventory_Implementation() final override;

	virtual FString GetInteractText() override;
	virtual bool ShouldGetOtherActorToInteractWith() override;

	virtual void Interact_Implementation(AActor* OtherActor) override;

private:
	void StartCrafting();
};
