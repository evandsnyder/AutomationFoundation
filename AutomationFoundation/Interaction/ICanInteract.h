#pragma once

#include "CoreMinimal.h"
#include "ICanInteract.generated.h"

class USoundCue;

UINTERFACE(MinimalAPI, Blueprintable)
class UCanInteract : public UInterface
{
	GENERATED_BODY()
};

class ICanInteract
{
	GENERATED_BODY()
public:
	virtual bool ShouldServerControlInteract();
	virtual bool ShouldGetOtherActorToInteractWith();
	
	virtual UAnimMontage* GetInteractAnimation();
	virtual USoundCue* GetInteractSound();
	virtual FVector GetRelativeInteractTextLocation();
	virtual FString GetInteractText();
	
	virtual void CancelCanInteract();
	virtual bool CanInteract();
};
