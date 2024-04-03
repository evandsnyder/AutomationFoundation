#pragma once
#include "AutomationFoundation/Inventory/InventoryComponent.h"

#include "TransporterBase.generated.h"

class UBoxComponent;

UCLASS()
class ATransporterBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> ItemSource;

	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> ItemDestination;
};
