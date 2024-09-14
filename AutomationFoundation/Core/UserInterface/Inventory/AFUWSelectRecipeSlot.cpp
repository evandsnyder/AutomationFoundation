#include "AFUWSelectRecipeSlot.h"

#include "AutomationFoundation/Core/UserInterface/AFButton.h"
#include "AutomationFoundation/Core/UserInterface/AFImage.h"

void UAFUWSelectRecipeSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectRecipeButton)
	{
		SelectRecipeButton->OnClicked.AddDynamic(this, &UAFUWSelectRecipeSlot::OnSelectRecipeButtonClicked);
	}
}

void UAFUWSelectRecipeSlot::SetRecipeSpecification(URecipeSpecification* NewSpecification)
{
	Specification = NewSpecification;
	Icon->SetBrushFromSoftTexture(Specification->Icon);
}

void UAFUWSelectRecipeSlot::OnSelectRecipeButtonClicked()
{
	RecipeSelected.Broadcast(Specification);
}
