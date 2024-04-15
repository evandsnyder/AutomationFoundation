#pragma once

#include "CoreMinimal.h"
#include "IInteractable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class IInteractable
{
	GENERATED_BODY()

public:
	virtual FText GetInteractionText() = 0;

	virtual void OnInteract(AActor* InteractInstigator) = 0;
};
