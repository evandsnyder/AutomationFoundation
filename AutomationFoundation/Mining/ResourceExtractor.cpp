#include "ResourceExtractor.h"

#include "ResourceNode.h"
#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Crafting/CraftingMachine.h"
#include "Components/BoxComponent.h"

AResourceExtractor::AResourceExtractor()
{
	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Extraction Machine Interact Box"));
	RootComponent = InteractBox;

	InteractBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AResourceExtractor::BeginActorOverlap);
	InteractBox->OnComponentEndOverlap.AddUniqueDynamic(this, &AResourceExtractor::EndActorOverlap);

	OutputInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Resource Extractor Output Inventory"));
	OutputInventory->SetInventorySize(1);
}

void AResourceExtractor::OnExtractorPlaced(AResourceNode* TargetResourceNode)
{
	ResourceNode = TargetResourceNode;

	if (!ResourceNode.IsValid())
	{
		LOG_WARNING(LogTemp, "Resource Extractor placed on Invalid Resource Node")
		return;
	}

	/*
	 * We only need to tie a recipe to a machine IF we decide we need input to a machine (such as water or fuel)
	AAutomationFoundationGameMode* GameMode = Cast<AAutomationFoundationGameMode>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		UItemSpecification* HarvestedResourceSpecification = ResourceNode->GetHarvestedResource();
		GameMode->FindRecipe()
	}
	*/

	StartMining();
}

void AResourceExtractor::OnMiningCycleComplete()
{
	UInventoryItemInstance* NewItem = NewObject<UInventoryItemInstance>();
	NewItem->ItemSpecification = ResourceNode->GetHarvestedResource();
	NewItem->CurrentStackSize = 1;

	OutputInventory->TryAddItemToInventory(NewItem);

	// Do not continue mining if the output item is full..
	if (IsOutputFull())
	{
		StopMining();
	}
}

void AResourceExtractor::OnOutputItemRemoved(UInventoryItemInstance* OldItem, int32 ItemIndex)
{
	if (!bIsMining && !IsOutputFull()) StartMining();
}

UInventoryComponent* AResourceExtractor::GetProviderInventory()
{
	return OutputInventory;
}

float AResourceExtractor::GetCraftingProgress() const
{
	if (!bIsMining) return 0.0f;

	return FMath::Clamp(GetWorldTimerManager().GetTimerElapsed(MiningTimerHandle) / MiningTime, 0.f, 1.f);
}

void AResourceExtractor::OnPlaced(AActor* PlacedOnActor)
{
	// IPlaceable::OnPlaced(PlacedOnActor);

	if (AResourceNode* Node = Cast<AResourceNode>(PlacedOnActor))
	{
		OnExtractorPlaced(Node);
	}
}

void AResourceExtractor::OnInteract(AActor* InteractInstigator)
{
	if (PlayerCharacter.IsValid())
	{
		PlayerCharacter->ToggleExtractionMachineUI();
	}
}

FText AResourceExtractor::GetInteractionText()
{
	return FText::FromString("Open Extraction Machine");
}

void AResourceExtractor::StartMining()
{
	GetWorldTimerManager().SetTimer(MiningTimerHandle, this, &AResourceExtractor::OnMiningCycleComplete, MiningTime,
	                                true);

	bIsMining = true;
}

void AResourceExtractor::StopMining()
{
	GetWorldTimerManager().ClearTimer(MiningTimerHandle);
	bIsMining = false;
}

bool AResourceExtractor::IsOutputFull() const
{
	// TODO: Implement this properly
	// For now, we're relying on there only being a SINGLE slot in the output inventory
	// If we switch to using a recipe-based system, we will need to refine this to properly validate all output ingredients...
	const UInventoryItemInstance* ItemInstance = OutputInventory->Inventory[0];
	return ItemInstance->CurrentStackSize >= ItemInstance->ItemSpecification->MaxStackSize;
}

void AResourceExtractor::BeginActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	PlayerCharacter = Cast<AAutomationFoundationCharacter>(OtherActor);
	if (!PlayerCharacter.IsValid()) return;

	PlayerCharacter->OnEnterActor(this);
	PlayerCharacter->ShowInteractPrompt();
}

void AResourceExtractor::EndActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerCharacter.IsValid())
	{
		PlayerCharacter->HideInteractPrompt();
		PlayerCharacter->OnLeaveActor();
		PlayerCharacter.Reset();
	}
}
