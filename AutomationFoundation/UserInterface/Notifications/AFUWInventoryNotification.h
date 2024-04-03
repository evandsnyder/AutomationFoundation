#pragma once

#include "AutomationFoundation/Core/UserInterface/AFNotification.h"
#include "AFUWInventoryNotification.generated.h"

class UInventoryItemInstance;

UCLASS()
class UAFUWInventoryNotification : public UAFNotification
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Configure(const UInventoryItemInstance* ItemInstance);
};
