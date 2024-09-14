#pragma once

#include "CoreMinimal.h"
#include "AFUWSelectRecipeSlot.h"
#include "AutomationFoundation/Core/UserInterface/AFUserWidget.h"
#include "AutomationFoundation/Crafting/CraftingMachineType.h"
#include "AFSelectRecipeWidget.generated.h"

class UWrapBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRecipeSelectedDelegate, URecipeSpecification*, NewRecipeSpecification);

UCLASS()
class UAFSelectRecipeWidget : public UAFUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	UWrapBox* AvailableRecipesGrid;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFUWSelectRecipeSlot> RecipeSlotClass;
	
public:
	virtual void WidgetActivated_Implementation() override;

	UPROPERTY(BlueprintAssignable)
	FRecipeSelectedDelegate OnRecipeSelected;

	void RefreshAvailableRecipes(ECraftingMachineType CraftingMachineType);

	UFUNCTION()
	void OnRecipeSlotSelected(URecipeSpecification* NewSpecification);
};
