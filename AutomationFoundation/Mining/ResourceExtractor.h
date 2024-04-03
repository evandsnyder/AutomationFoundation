#pragma once
#include "AutomationFoundation/BuildSystem/IPlaceable.h"
#include "AutomationFoundation/Crafting/CraftingMachineType.h"
#include "AutomationFoundation/Crafting/RecipeSpecification.h"
#include "AutomationFoundation/Interaction/IInteractable.h"
#include "AutomationFoundation/Inventory/IItemProvider.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"

#include "ResourceExtractor.generated.h"

class UBoxComponent;
class AAutomationFoundationCharacter;
class AResourceNode;

/**
 * We could make this class inherit from the Crafting Machine, but the Crafting Machine has a lot
 * of extra fluff that we don't really need (for now) such as ingredient inputs and so forth
 */

UCLASS()
class AResourceExtractor : public AActor, public IPlaceable, public IItemProvider, public IInteractable
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TWeakObjectPtr<AResourceNode> ResourceNode;

	UPROPERTY(BlueprintReadOnly)
	ECraftingMachineType MachineType = ECraftingMachineType::ResourceExtractor;

	UPROPERTY()
	FRecipeSpecification CurrentRecipe;

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* OutputInventory;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle MiningTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MiningTime = 1.0f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsMining = false;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* InteractBox;
	TWeakObjectPtr<AAutomationFoundationCharacter> PlayerCharacter;

public:
	UFUNCTION()
	void BeginActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	AResourceExtractor();

	UFUNCTION(BlueprintCallable)
	void OnExtractorPlaced(AResourceNode* TargetResourceNode);

	UFUNCTION()
	void OnMiningCycleComplete();

	UFUNCTION()
	void OnOutputItemRemoved(UInventoryItemInstance* OldItem, int32 ItemIndex);

	UFUNCTION()
	virtual UInventoryComponent* GetProviderInventory() override;

	UFUNCTION(BlueprintCallable)
	float GetCraftingProgress() const;

	virtual void OnPlaced(AActor* PlacedOnActor) override;

	virtual void OnInteract() override;
	virtual FText GetInteractionText() override;

private:
	void StartMining();
	void StopMining();

	bool IsOutputFull() const;
};
