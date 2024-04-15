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

void AItemPickup::OnPlayerBeginOverlap()
{
	if (PlayerCharacter)
	{
		// Show the Interact Prompt...
		UE_LOG(LogTemp, Display, TEXT("Player Entered Interaction Zone"));
		PlayerCharacter->ShowInteractPrompt();
	}
}

void AItemPickup::OnPlayerEndOverlap()
{
	if (PlayerCharacter)
	{
		// Remove the Interact Prompt
		UE_LOG(LogTemp, Display, TEXT("Player Left Interaction Zone"));
		PlayerCharacter->HideInteractPrompt();
	}
}

void AItemPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AItemPickup::OnInteract_Implementation(AActor* InteractInstigator)
{
	if (PlayerCharacter)
	{
		if (PlayerCharacter->AddItemToInventory(Item))
		{
			// We can delete this actor..
			Destroy();
			OnItemCollected();
		}
	}
}

FText AItemPickup::GetInteractionText()
{
	return FText::FromString(TEXT("Press 'E' to collect item"));
}
