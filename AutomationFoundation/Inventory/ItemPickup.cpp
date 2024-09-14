#include "ItemPickup.h"

#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "AutomationFoundation/Core/AutomationFoundationGlobals.h"
#include "Engine/StreamableManager.h"

AItemPickup::AItemPickup()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void AItemPickup::ConfigureItemPickup(UInventoryItemInstance* ItemInstance)
{
	Item = ItemInstance;
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Intialized Item Pickup with a null ItemInstance"));
		return;
	}

	if (UStaticMesh* DisplayMesh = Item->ItemSpecification->DisplayMesh.LoadSynchronous())
	{
		UE_LOG(LogTemp, Display, TEXT("Loaded static mesh"));
		StaticMesh->SetStaticMesh(DisplayMesh);
		StaticMesh->SetHiddenInGame(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Static Mesh was still null"));
	}
}

void AItemPickup::BeginPlay()
{
	Super::BeginPlay();
}
