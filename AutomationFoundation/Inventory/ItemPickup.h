#pragma once

#include "ItemSpecification.h"
#include "InventoryItemInstance.h"
#include "ItemSpecificationDataTableReference.h"
#include "AutomationFoundation/Interaction/IInteractable.h"
#include "AutomationFoundation/Interaction/PickupBase.h"
#include "ItemPickup.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AItemPickup : public APickupBase, public IInteractable
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInventoryItemInstance> Item;

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

public:
	AItemPickup();

	UFUNCTION(BlueprintCallable)
	void ConfigureItemPickup(UInventoryItemInstance* ItemInstance);

protected:
	// APickupBase Interface
	virtual void OnPlayerBeginOverlap() override;
	virtual void OnPlayerEndOverlap() override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnItemCollected();

public:
	// IInteractable Interface
	virtual FText GetInteractionText() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void OnInteract();
	void OnInteract_Implementation();
};
