#include "AFSelectRecipeWidget.h"

#include "AFUWSelectRecipeSlot.h"
#include "AutomationFoundation/Core/AutomationFoundationGameMode.h"
#include "Components/WrapBox.h"

void UAFSelectRecipeWidget::WidgetActivated()
{
	Super::WidgetActivated();
}

void UAFSelectRecipeWidget::RefreshAvailableRecipes(ECraftingMachineType CraftingMachineType)
{
	AvailableRecipesGrid->ClearChildren(); // Do we need to remove all dynamic subscriptions?

	AAutomationFoundationGameMode* GameMode = Cast<AAutomationFoundationGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get game mode for Recipe Select Widget"))
		return;
	}

	// Need to convert the crafting machine type to the filter type..

	ECraftingMachineFilterType Filter;
	switch(CraftingMachineType)
	{
	case ECraftingMachineType::Basic:
		Filter = ECraftingMachineFilterType::Basic;
		break;
	case ECraftingMachineType::Chemical:
		Filter = ECraftingMachineFilterType::Chemical;
		break;
	default:
		Filter = ECraftingMachineFilterType::InventoryOnly;
		break;
	}

	for(FRecipeSpecification Specification : GameMode->FindAllRecipes(Filter))
	{
		UAFUWSelectRecipeSlot* RecipeSlot = CreateWidget<UAFUWSelectRecipeSlot>(this, RecipeSlotClass);
		RecipeSlot->SetRecipeSpecification(Specification);
		RecipeSlot->RecipeSelected.AddDynamic(this, &UAFSelectRecipeWidget::OnRecipeSlotSelected);

		AvailableRecipesGrid->AddChildToWrapBox(RecipeSlot);
	}

}

void UAFSelectRecipeWidget::OnRecipeSlotSelected(const FRecipeSpecification& NewSpecification)
{
	OnRecipeSelected.Broadcast(NewSpecification);
	SetVisibility(ESlateVisibility::Collapsed);
	WidgetDeactivated();
}
