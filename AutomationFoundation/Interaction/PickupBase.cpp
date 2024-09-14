#include "PickupBase.h"

#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "Components/SphereComponent.h"

APickupBase::APickupBase()
{
	// No need for this to tick
	PrimaryActorTick.bCanEverTick = false;
}
