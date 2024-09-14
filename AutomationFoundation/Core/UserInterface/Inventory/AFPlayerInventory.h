#pragma once

#include "AFInventory.h"
#include "AFPlayerInventory.generated.h"

UCLASS()
class UAFPlayerInventory : public UAFInventory
{
	GENERATED_BODY()
public:
	virtual void WidgetActivated_Implementation() override;
};
