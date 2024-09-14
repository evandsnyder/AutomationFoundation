#pragma once
#include "AutomationFoundation/BuildSystem/Buildables/Machine.h"
#include "AutomationFoundation/Crafting/CraftingMachineType.h"

#include "ResourceExtractor.generated.h"

class UBoxComponent;
class AAutomationFoundationCharacter;
class AResourceNode;

UCLASS()
class AResourceExtractor : public AMachine
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	ECraftingMachineType MachineType = ECraftingMachineType::ResourceExtractor;

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* Inventory;

	TWeakObjectPtr<AAutomationFoundationCharacter> PlayerCharacter;

public:
	AResourceExtractor();

	UFUNCTION()
	void OnOutputItemRemoved(UInventoryItemInstance* OldItem, int32 ItemIndex);

	UFUNCTION()
	virtual UInventoryComponent* GetProviderInventory_Implementation() override;
	virtual UInventoryComponent* GetAcceptorInventory_Implementation() override;

	virtual bool IsPlaceable_Implementation() override;
	virtual void UpdateLocation_Implementation(const FHitResult& HitResult) override;

	virtual void CreatePreview_Implementation() override;
	virtual void ScrollPreview_Implementation(float Value) override;
	virtual void OnPreviewPlaced_Implementation() override;

	virtual bool CanOutput_Implementation() override;
};
