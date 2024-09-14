#pragma once

#include "IItemProvider.generated.h"

class UInventoryComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UItemProvider : public UInterface
{
	GENERATED_BODY()
};

class IItemProvider
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UInventoryComponent* GetProviderInventory();
};
