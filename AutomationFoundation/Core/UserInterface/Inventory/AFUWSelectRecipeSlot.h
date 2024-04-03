#pragma once

#include "CoreMinimal.h"
#include "AutomationFoundation/Core/UserInterface/AFUserWidget.h"
#include "AutomationFoundation/Crafting/RecipeSpecification.h"
#include "AFUWSelectRecipeSlot.generated.h"

class UAFButton;
class UAFImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRecipeSlotSelectedDelegate, const FRecipeSpecification&, NewRecipeSpecification);

UCLASS()
class UAFUWSelectRecipeSlot : public UAFUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta=(BindWidget))
	UAFImage* Icon;

	UPROPERTY(meta=(BindWidget))
	UAFButton* SelectRecipeButton;

	UPROPERTY()
	FRecipeSpecification Specification;
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintAssignable)
	FRecipeSlotSelectedDelegate RecipeSelected;

	void SetRecipeSpecification(const FRecipeSpecification& NewSpecification);

private:
	UFUNCTION()
	void OnSelectRecipeButtonClicked();
};
