#include "PickupBase.h"

#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "Components/SphereComponent.h"

APickupBase::APickupBase()
{
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	RootComponent = CollectionSphere;

	CollectionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &APickupBase::BeginOverlap);
	CollectionSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &APickupBase::EndOverlap);
	
	// No need for this to tick
	PrimaryActorTick.bCanEverTick = false;
}

void APickupBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerCharacter = Cast<AAutomationFoundationCharacter>(OtherActor);

	if(!PlayerCharacter) return;

	// Tell the character we are in interact range...
	PlayerCharacter->OnEnterActor(this);
	OnPlayerBeginOverlap();
}

void APickupBase::OnPlayerBeginOverlap()
{
}

void APickupBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if(PlayerCharacter)
	{
		PlayerCharacter->OnLeaveActor();
		OnPlayerEndOverlap();
	}
}

void APickupBase::OnPlayerEndOverlap()
{
}
