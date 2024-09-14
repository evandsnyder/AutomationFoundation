#pragma once

#include "IInteract.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteract : public UInterface
{
	GENERATED_BODY()
};

class IInteract
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(AActor* OtherActor);
};
