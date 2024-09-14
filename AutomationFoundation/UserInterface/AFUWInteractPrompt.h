#pragma once

#include "AutomationFoundation/Core/UserInterface/AFUserWidget.h"
#include "AFUWInteractPrompt.generated.h"

class UAFImage;
class UAFTextBlock;

UCLASS()
class UAFUWInteractPrompt : public UAFUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget), EditAnywhere)
	TObjectPtr<UAFTextBlock> InteractLabel;

	UPROPERTY(meta=(BindWidget), EditAnywhere)
	TObjectPtr<UAFImage> ButtonIcon;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateButtonIcon(UAFImage* NewIcon);

	UFUNCTION(BlueprintCallable)
	void UpdateInteractText(const FText& NewText);
};
