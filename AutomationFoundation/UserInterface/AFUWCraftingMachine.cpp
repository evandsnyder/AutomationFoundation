#include "AFUWCraftingMachine.h"

#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Core/UserInterface/AFButton.h"
#include "AutomationFoundation/Core/UserInterface/AFTextBlock.h"
#include "AutomationFoundation/Core/UserInterface/Inventory/AFInventorySlot.h"
#include "AutomationFoundation/Core/UserInterface/Inventory/AFSelectRecipeWidget.h"
#include "AutomationFoundation/Crafting/CraftingMachine.h"
#include "AutomationFoundation/Interaction/InteractComponent.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WrapBox.h"

void UAFUWCraftingMachine::NativeConstruct()
{
	Super::NativeConstruct();

	if (RecipeNameLabel)
	{
		RecipeNameLabel->SetText(FText::FromString(TEXT("No Recipe Set")));
	}

	if (ChangeRecipeButton)
	{
		ChangeRecipeButton->OnClicked.AddDynamic(this, &UAFUWCraftingMachine::OnChangeRecipeButtonClicked);
	}
}

void UAFUWCraftingMachine::OnCraftingRecipeChanged(URecipeSpecification* NewSpecification)
{
	OwningMachine->SetCraftingRecipe(NewSpecification);
	// If there are any items in the input or output ingredients, move them over to the player's inventory
	// delete the old inventory slots
	// create new ones

	LOG_INFO(LogTemp, "Changed Recipe");
	RefreshUI();
}

void UAFUWCraftingMachine::RefreshUI()
{
	// create child widgets
	// bind events to other events...
	if (!OwningMachine.IsValid()) return;

	InputIngredientGrid->ClearChildren();
	OutputIngredientGrid->ClearChildren();

	if (!OwningMachine->CurrentRecipe || OwningMachine->CurrentRecipe->RecipeID == "UNKNOWN") return;

	RecipeNameLabel->SetText(OwningMachine->CurrentRecipe->RecipeName);

	OwningMachine->InputInventory->OnItemChanged.AddUniqueDynamic(this, &UAFUWCraftingMachine::OnInputItemChanged);
	OwningMachine->OutputInventory->OnItemChanged.AddUniqueDynamic(this, &UAFUWCraftingMachine::OnOutputItemChanged);

	// NOTE: Filters  are already set in the inventory. We just need to apply those to the inventory slot...

	// Okay... Need to create the slot widgets
	int i = 0;
	for (const TPair<FName, int32>& Element : OwningMachine->CurrentRecipe->InputItems)
	{
		UAFInventorySlot* NewInputWidget = CreateWidget<UAFInventorySlot>(this, InventorySlotWidget);
		NewInputWidget->SetItemSlot(i);
		NewInputWidget->SetOwningInventory(OwningMachine->InputInventory);
		NewInputWidget->ApplyFilter(Element.Key);
		InputIngredientGrid->AddChildToWrapBox(NewInputWidget);
		i++;
	}

	i = 0;
	for (const TPair<FName, int32>& Element : OwningMachine->CurrentRecipe->OutputItems)
	{
		UAFInventorySlot* NewOutputWidget = CreateWidget<UAFInventorySlot>(this, InventorySlotWidget);
		NewOutputWidget->SetItemSlot(i);
		NewOutputWidget->SetOwningInventory(OwningMachine->OutputInventory);
		NewOutputWidget->ApplyFilter(Element.Key);
		OutputIngredientGrid->AddChildToWrapBox(NewOutputWidget);
		i++;
	}
}

void UAFUWCraftingMachine::OnInputItemChanged(UInventoryItemInstance* Item, int32 ItemIndex)
{
	UAFInventorySlot* ItemSlot = Cast<UAFInventorySlot>(InputIngredientGrid->GetChildAt(ItemIndex));
	if (IsValid(ItemSlot))
	{
		ItemSlot->ConfigureSlot(Item, ItemIndex);
	}
}

void UAFUWCraftingMachine::OnOutputItemChanged(UInventoryItemInstance* Item, int32 ItemIndex)
{
	UAFInventorySlot* ItemSlot = Cast<UAFInventorySlot>(OutputIngredientGrid->GetChildAt(ItemIndex));
	if (IsValid(ItemSlot))
	{
		ItemSlot->ConfigureSlot(Item, ItemIndex);
	}
}

void UAFUWCraftingMachine::WidgetActivated_Implementation()
{
	// Re-bind Widget..
	const AAutomationFoundationCharacter* Player = Cast<AAutomationFoundationCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owning Player Pawn was not the expected character type. Could not bind crafting machine to widget"));
		return;
	}

	UInteractComponent* InteractComponent = Player->GetComponentByClass<UInteractComponent>();

	if (!IsValid(InteractComponent))
	{
		LOG_WARNING(LogTemp, "Could not get the interact component from the player");
		return;
	}

	OwningMachine = Cast<ACraftingMachine>(InteractComponent->GetInteractTarget());
	if (!OwningMachine.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not bind to machine. Invalid Actor pointer"));
		return;
	}

	RefreshUI();
}

void UAFUWCraftingMachine::WidgetDeactivated_Implementation()
{
	// Need to unsubscribe from Item Handlers..

	if (OwningMachine.IsValid())
	{
		OwningMachine->InputInventory->OnItemChanged.RemoveDynamic(this, &UAFUWCraftingMachine::OnInputItemChanged);
		OwningMachine->OutputInventory->OnItemChanged.RemoveDynamic(this, &UAFUWCraftingMachine::OnOutputItemChanged);
		OwningMachine.Reset();
	}
}

void UAFUWCraftingMachine::NativeTick(const FGeometry& InGeometry, float InDeltaTime)
{
	Super::NativeTick(InGeometry, InDeltaTime);
	if (OwningMachine.IsValid() && CraftingProgressBar)
	{
		CraftingProgressBar->SetPercent(OwningMachine->GetCraftingProgress());
	}
}

void UAFUWCraftingMachine::OnChangeRecipeButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Changing Recipe on selected machine..."));
	}
	// Activate the Recipe Select widget...
	if (!IsValid(SelectRecipeWidget))
	{
		// Create the widget...
		SelectRecipeWidget = CreateWidget<UAFSelectRecipeWidget>(this, SelectRecipeClass);
		// Maybe need that widget to close?
		SelectRecipeWidget->OnRecipeSelected.AddDynamic(this, &UAFUWCraftingMachine::OnCraftingRecipeChanged);
		SelectRecipeWidget->AddToViewport();
	}

	SelectRecipeWidget->SetVisibility(ESlateVisibility::Visible);
	SelectRecipeWidget->WidgetActivated_Implementation();
	if (OwningMachine.IsValid())
	{
		SelectRecipeWidget->RefreshAvailableRecipes(OwningMachine->GetMachineType());
	}
}
