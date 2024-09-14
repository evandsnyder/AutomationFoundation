#pragma once

#include "CoreMinimal.h"
#include "PickupBase.generated.h"

UCLASS()
class APickupBase : public AActor
{
	GENERATED_BODY()

protected:
	TObjectPtr<class AAutomationFoundationCharacter> PlayerCharacter;

public:
	APickupBase();
};
