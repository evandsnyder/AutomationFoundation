#include "ICanInteract.h"

bool ICanInteract::ShouldServerControlInteract()
{
	return false;
}

bool ICanInteract::ShouldGetOtherActorToInteractWith()
{
	return true;
}

UAnimMontage* ICanInteract::GetInteractAnimation()
{
	return nullptr;
}

USoundCue* ICanInteract::GetInteractSound()
{
	return nullptr;
}

FVector ICanInteract::GetRelativeInteractTextLocation()
{
	return {};
}

FString ICanInteract::GetInteractText()
{
	return "Interact";
}

void ICanInteract::CancelCanInteract()
{
}

bool ICanInteract::CanInteract()
{
	return true;
}
