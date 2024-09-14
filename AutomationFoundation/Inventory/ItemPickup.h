#pragma once

#include "ItemSpecification.h"
#include "InventoryItemInstance.h"
#include "ItemSpecificationDataTableReference.h"
#include "AutomationFoundation/Interaction/IInteractable.h"
#include "AutomationFoundation/Interaction/PickupBase.h"
#include "ItemPickup.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AItemPickup : public APickupBase
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

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnItemCollected();
};
