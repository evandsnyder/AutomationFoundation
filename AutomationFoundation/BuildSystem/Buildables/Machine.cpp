#include "Machine.h"

#include "ConveyorBelt.h"
#include "AutomationFoundation/BuildSystem/AttachPoint.h"
#include "AutomationFoundation/BuildSystem/ConveyorItem.h"
#include "AutomationFoundation/Core/AutomationFoundationGameMode.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "AutomationFoundation/Inventory/InventoryItemInstance.h"
#include "Kismet/GameplayStatics.h"

AMachine::AMachine()
{
}

void AMachine::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> ChildActors;
	GetAllChildActors(ChildActors);

	// Need to ignore these actors for placement...

	for (AActor* Actor : ChildActors)
	{
		if (AAttachPoint* AttachPoint = Cast<AAttachPoint>(Actor))
		{
			AttachPoint->SetParentBuild(this);
			LOG_INFO(LogTemp, "Adding Ignored Attach Point for Placement");
			ActorsToIgnoreDuringPlacement.Add(AttachPoint);
		}
	}
}

void AMachine::ConnectAttachPointAsInput(AAttachPoint* AttachPoint)
{
	// This function may no longer be necessary for this class?
	// TODO: Review later and determine necessity
	if (AConveyorBelt* Conveyor = Cast<AConveyorBelt>(AttachPoint->GetConnectedBuild()))
	{
		if (!Inputs.Contains(AttachPoint))
		{
			Inputs.Add(AttachPoint, Execute_GetAcceptorInventory(this));
		}
	}
}

void AMachine::ConnectAttachPointAsOutput(AAttachPoint* AttachPoint)
{
	if (AConveyorBelt* Conveyor = Cast<AConveyorBelt>(AttachPoint->GetConnectedBuild()))
	{
		if (!Outputs.Contains(AttachPoint))
		{
			Outputs.Add(AttachPoint, Execute_GetProviderInventory(this));
		}
		StartOutputting();
	}
}

void AMachine::StartOutputting_Implementation()
{
	bCanOutputNextItem = true;

	UInventoryComponent* OutputInventory = Execute_GetProviderInventory(this);

	if (Outputs.IsEmpty())
	{
		return;
	}

	for (auto& [AttachPoint, Value] : Outputs)
	{
		int32 FirstItemIndex = INDEX_NONE;
		UInventoryItemInstance* Item = OutputInventory->FindFirstItem(FirstItemIndex);

		if (IsValid(Item))
		{
			if (OutputFromAttachPoint(AttachPoint, Item->ItemSpecification))
			{
				bCanOutputNextItem = false;
				// Handle successful output...
				Item->CurrentStackSize -= 1;
				if (Item->CurrentStackSize <= 0)
				{
					OutputInventory->RemoveItemFromInventory(Item);
				}
			}
		}
	}
}

bool AMachine::OutputFromAttachPoint(AAttachPoint* OutputAttachPoint, UItemSpecification* ItemSpecification)
{
	if (!CanOutput())
	{
		return false;
	}

	if (AConveyorBelt* ConveyorBelt = Cast<AConveyorBelt>(OutputAttachPoint->GetConnectedBuild()))
	{
		if (ConveyorBelt->CanSpawnNextItem())
		{
			const FTransform SpawnTransform = ConveyorBelt->GetTransformByAlpha(0.0f);

			if (AConveyorItem* NewItem = GetWorld()->SpawnActorDeferred<AConveyorItem>(ConveyorItemClass, SpawnTransform))
			{
				NewItem->SetConveyor(ConveyorBelt);
				NewItem->SetItemSpecification(ItemSpecification);
				NewItem->SetItemInterval(ItemSpawnInterval);

				UGameplayStatics::FinishSpawningActor(NewItem, SpawnTransform);
				NewItem->OnSpawnNext.AddDynamic(this, &AMachine::StartOutputting);
				return true;
			}
		}
	}

	return false;
}

bool AMachine::TryAcceptInput_Implementation(AAttachPoint* ProvidingAttachPoint, AConveyorItem* IncomingItem)
{
	// Do we have an inventory that matches this input?
	// if so, we need to try to add the item to it...
	UInventoryComponent* Inventory = Execute_GetAcceptorInventory(this);
	if (IsValid(Inventory))
	{
		UInventoryItemInstance* NewItem = NewObject<UInventoryItemInstance>();
		NewItem->ItemSpecification = IncomingItem->GetItemSpecification();
		LOG_INFO(LogTemp, "Attempting to Add Item to Inventory");
		if (Inventory->TryAddItemToInventory(NewItem))
		{
			if (CanOutput())
				StartOutputting();
			return true;
		}
	}
	else
	{
		LOG_ERROR(LogTemp, "Acceptor Inventory Is Invalid");
	}

	return false;
}

bool AMachine::CanOutput_Implementation()
{
	return bCanOutputNextItem;
}

FString AMachine::GetInteractText()
{
	return ICanInteract::GetInteractText();
}

bool AMachine::CanInteract()
{
	return bIsPlaced;
}

bool AMachine::PlacePreview_Implementation()
{
	bIsPlaced = true;
	return Super::PlacePreview_Implementation();
}

void AMachine::StartCrafting()
{
	// Remove the Input ingredients from the input inventory
	if (CanCraft())
	{
		// TODO: "suspend" the input ingredients... (If there are any)
		bIsCrafting = true;
		GetWorldTimerManager().SetTimer(CraftingTimer, this, &AMachine::OnCraftingComplete, CurrentRecipe->CraftingTime);
	}
}

void AMachine::CancelCraftingProcess()
{
	// Stop the timer
	bIsCrafting = false;
	GetWorldTimerManager().ClearTimer(CraftingTimer);
	// Add the ingredients back into input
}

float AMachine::GetCraftingProgress() const
{
	if (!bIsCrafting) return 0.0f;
	return FMath::Clamp(GetWorldTimerManager().GetTimerElapsed(CraftingTimer) / CurrentRecipe->CraftingTime, 0.0f, 1.0f);
}

bool AMachine::CanCraft()
{
	return !bIsCrafting && InputIngredientsAreSatisfied() && !IsOutputFull();
}

bool AMachine::InputIngredientsAreSatisfied()
{
	if (CurrentRecipe->InputItems.IsEmpty()) return true;
	UInventoryComponent* InputInventory = IItemAcceptor::Execute_GetAcceptorInventory(this);

	for (const UInventoryItemInstance* ItemInstance : InputInventory->Inventory)
	{
		const FName& ItemID = ItemInstance->ItemSpecification->ItemID;
		if (!CurrentRecipe->InputItems.Contains(ItemID) || ItemInstance->CurrentStackSize < CurrentRecipe->InputItems[ItemID])
		{
			return false;
		}
	}

	return true;
}

bool AMachine::IsOutputFull()
{
	UInventoryComponent* Output = Execute_GetProviderInventory(this);
	for (const UInventoryItemInstance* ItemInstance : Output->Inventory)
	{
		if (IsValid(ItemInstance) &&
			ItemInstance->CurrentStackSize >=
			(ItemInstance->ItemSpecification->MaxStackSize - CurrentRecipe->OutputItems[ItemInstance->ItemSpecification->ItemID]))
			return true;
	}

	return false;
}

void AMachine::OnCraftingComplete_Implementation()
{
	bIsCrafting = false;
	UInventoryComponent* OutputInventory = Execute_GetProviderInventory(this);

	AAutomationFoundationGameMode* GameMode = Cast<AAutomationFoundationGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode))
	{
		LOG_ERROR(LogTemp, "Failed To GetAppropriate Game Mode");
		return;
	}

	// Add the new items to the output inventory
	for (TPair<FName, int32> OutputItem : CurrentRecipe->OutputItems)
	{
		FItemSpecificationDataTableReference ItemSpecRef;
		GameMode->FindItem(OutputItem.Key, ItemSpecRef);
		UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>();
		ItemInstance->ItemSpecification = ItemSpecRef.ItemSpecificationRef.LoadSynchronous();
		ItemInstance->CurrentStackSize = OutputItem.Value;
		if (!OutputInventory->TryAddItemToInventory(ItemInstance))
		{
			LOG_ERROR(LogTemp, "Failed to Add crafted Item to inventory");
		}
	}

	StartCrafting();

	if (CanOutput())
	{
		StartOutputting();
	}
}

void AMachine::UpdateRecipe_Implementation(URecipeSpecification* NewRecipe)
{
	CurrentRecipe = NewRecipe;
	if (UInventoryComponent* AcceptorInventory = Execute_GetAcceptorInventory(this))
	{
		AcceptorInventory->ConfigureInventoryFromRecipe(NewRecipe);
	}

	if (
		UInventoryComponent* ProviderInventory = Execute_GetProviderInventory(this))
	{
		ProviderInventory->ConfigureInventoryFromRecipe(NewRecipe, false);
	}
}
