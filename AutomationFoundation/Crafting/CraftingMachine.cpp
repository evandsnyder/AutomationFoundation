﻿#include "CraftingMachine.h"

#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "AutomationFoundation/Core/AutomationFoundationGameMode.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ACraftingMachine::ACraftingMachine() : ACraftingMachineBase(ECraftingMachineType::Basic)
{
	InputInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Input Ingredients"));
	OutputInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Output Ingredients"));
}

void ACraftingMachine::BeginPlay()
{
	Super::BeginPlay();

	if (InputInventory)
	{
		InputInventory->OnItemChanged.AddUniqueDynamic(this, &ACraftingMachine::OnInputItemAdded);
	}
}

void ACraftingMachine::SetCraftingRecipe(URecipeSpecification* NewRecipe)
{
	CurrentRecipe = NewRecipe;

	// TODO: Need a refresh behavior
	// Move all input/output ingredients to the player inventory
	// ensure our inventory is empty

	int32 i = 0;
	InputInventory->SetInventorySize(CurrentRecipe->InputItems.Num());
	for (auto const& [ItemID, _] : CurrentRecipe->InputItems)
	{
		InputInventory->AddInventoryFilter(ItemID, i);
		i++;
	}

	OutputInventory->SetInventorySize(CurrentRecipe->OutputItems.Num());
	for (auto const& [ItemID, _] : CurrentRecipe->OutputItems)
	{
		OutputInventory->AddInventoryFilter(ItemID, i);
		i++;
	}
}

void ACraftingMachine::OnCraftingComplete_Implementation()
{
	bIsCrafting = false;

	AAutomationFoundationGameMode* GameMode = Cast<AAutomationFoundationGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get GameMode when completing crafting recipe"));
		return;
	}

	bCanCraft = true; // we're going to reset this...

	// Remove items from the input inventory...
	for (UInventoryItemInstance* InputItem : InputInventory->Inventory)
	{
		InputItem->CurrentStackSize -= CurrentRecipe->InputItems[InputItem->ItemSpecification->ItemID];
		InputInventory->OnItemChanged.Broadcast(InputItem, 0);
	}

	// Add output Items to the Output Inventory
	for (const auto& OutputItem : CurrentRecipe->OutputItems)
	{
		// Lookup the ItemSpecification
		FItemSpecificationDataTableReference ItemSpecRef;
		if (!GameMode->FindItem(OutputItem.Key, ItemSpecRef))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find the output item expected in recipe"));
			return;
		}

		UItemSpecification* ItemSpecification = ItemSpecRef.ItemSpecificationRef.LoadSynchronous();

		UInventoryItemInstance* OutputInstance = NewObject<UInventoryItemInstance>();
		OutputInstance->ItemSpecification = ItemSpecification;
		OutputInstance->CurrentStackSize = OutputItem.Value;
		OutputInventory->TryAddItemToInventory(OutputInstance);

		// One of these Inventory's will be at MOST 3 or 4 in size and will only contain a single stack of each item...
		// So the cost of scanning is negligible.
		const UInventoryItemInstance* ExistingOutputItem = OutputInventory->FindFirstInstanceOrNull(ItemSpecification->ItemID);
		if (IsValid(ExistingOutputItem))
		{
			bCanCraft &= ExistingOutputItem->CurrentStackSize + OutputItem.Value <= OutputInstance->ItemSpecification->MaxStackSize;
		}
	}

	bCanCraft &= AreInputIngredientsSatisfied();
	if (bCanCraft)
	{
		StartCrafting();
	}
}

void ACraftingMachine::OnInputItemAdded_Implementation(UInventoryItemInstance* NewItem, int32 ItemIndex)
{
	if (bIsCrafting) return; // No need to process anymore logic if we're already crafting

	bCanCraft = CanCraft();
	if (bCanCraft) StartCrafting();
}

void ACraftingMachine::OnOutputItemRemoved(UInventoryItemInstance* RemovedItem, int32 OldIndex)
{
	// This is useful if we are already waiting on the output to drain before we craft any more
	if (!bIsCrafting && !IsOutputFull())
		StartCrafting();
}

/**
 * Checks whether we have all the necessary InputIngredients to run a crafting cycle
 * @return 
 */
bool ACraftingMachine::AreInputIngredientsSatisfied() const
{
	for (const UInventoryItemInstance* ItemInstance : InputInventory->Inventory)
	{
		if(!IsValid(ItemInstance))
		{
			return false;
		}
		const FName& ItemID = ItemInstance->ItemSpecification->ItemID;
		if (!CurrentRecipe->InputItems.Contains(ItemID) || ItemInstance->CurrentStackSize < CurrentRecipe->InputItems[ItemID])
		{
			return false;
		}
	}

	return true;
}

/**
 * Checks whether we have room in the output to drop another stack of constructed items
 * @return 
 */
bool ACraftingMachine::IsOutputFull() const
{
	for (const UInventoryItemInstance* ItemInstance : OutputInventory->Inventory)
	{
		if (IsValid(ItemInstance) &&
			ItemInstance->CurrentStackSize >=
			(ItemInstance->ItemSpecification->MaxStackSize - CurrentRecipe->OutputItems[ItemInstance->ItemSpecification->ItemID])
		)
			return true;
	}

	return false;
}

bool ACraftingMachine::CanCraft() const
{
	return !bIsCrafting && AreInputIngredientsSatisfied() && !IsOutputFull();
}

float ACraftingMachine::GetCraftingProgress() const
{
	if (!bIsCrafting) return 0.0f;

	return FMath::Clamp(GetWorldTimerManager().GetTimerElapsed(CraftingTimer) / CurrentRecipe->CraftingTime, 0.0f, 1.0f);
}

UInventoryComponent* ACraftingMachine::GetAcceptorInventory_Implementation()
{
	return InputInventory;
}

UInventoryComponent* ACraftingMachine::GetProviderInventory_Implementation()
{
	return OutputInventory;
}

FString ACraftingMachine::GetInteractText()
{
	return "Open Crafting Machine";
}

void ACraftingMachine::Interact_Implementation(AActor* OtherActor)
{
	if (AAutomationFoundationCharacter* Character = Cast<AAutomationFoundationCharacter>(OtherActor))
	{
		Character->ToggleCraftingMachineUI();
	}
}

bool ACraftingMachine::ShouldGetOtherActorToInteractWith()
{
	return true;
}

void ACraftingMachine::StartCrafting()
{
	// Maybe move required ingredients into a special, temporary place? (like a crafting queue)
	bIsCrafting = true;
	GetWorldTimerManager().SetTimer(CraftingTimer, this, &ACraftingMachine::OnCraftingComplete,
	                                CurrentRecipe->CraftingTime);
}
