#pragma once

#include "IPlaceable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPlaceable : public UInterface
{
	GENERATED_BODY()
};

class IPlaceable
{
	GENERATED_BODY()

public:
	virtual void OnPlaced(AActor* PlacedOnActor = nullptr);
};
