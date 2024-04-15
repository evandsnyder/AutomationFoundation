#pragma once

#include "CoreMinimal.h"
#include "CraftingMachineBase.h"
#include "RecipeSpecification.h"
#include "AutomationFoundation/Interaction/IInteractable.h"
#include "AutomationFoundation/Inventory/IItemAcceptor.h"
#include "AutomationFoundation/Inventory/IItemProvider.h"
#include "AutomationFoundation/Inventory/InventoryItemInstance.h"
#include "CraftingMachine.generated.h"

UENUM(BlueprintType)
enum class EMachineStatus
{
	Unknown,
	NoRecipeSet,
	UnneededIngredient,
	InventoryFull,
	ItemConsumedComplete,
	ItemConsumedPartial
};

UCLASS(BlueprintType, Blueprintable)
class ACraftingMachine : public ACraftingMachineBase, public IItemAcceptor, public IItemProvider
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FRecipeSpecification CurrentRecipe;

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

private:
	TObjectPtr<class AAutomationFoundationCharacter> PlayerCharacter;

public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* InteractBox;

	ACraftingMachine();

	virtual void BeginPlay() override;

	UFUNCTION()
	void SetCraftingRecipe(const FRecipeSpecification& NewRecipe);

	UFUNCTION(BlueprintNativeEvent)
	void OnCraftingComplete();


	virtual void OnInteract(AActor* InteractInstigator) override;

	virtual FText GetInteractionText() override;

	UFUNCTION()
	virtual void BeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void EndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex
	);

	UFUNCTION()
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
	virtual UInventoryComponent* GetAcceptorInventory() final override;

	UFUNCTION()
	virtual UInventoryComponent* GetProviderInventory() final override;

private:
	void StartCrafting();
};
