#pragma once

#include "IItemAcceptor.generated.h"

class UInventoryComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UItemAcceptor : public UInterface
{
	GENERATED_BODY()
};

class IItemAcceptor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UInventoryComponent* GetAcceptorInventory();
};
