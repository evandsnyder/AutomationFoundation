#pragma once

#include "AutomationFoundation/Core/UserInterface/AFUserWidget.h"
#include "AFUWHealthBar.generated.h"

class UHealthComponent;
class UProgressBar;

UCLASS()
class UAFUWHealthBar : public UAFUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar;
	
public:
	UFUNCTION()
	void OnHealthChanged(AActor* InInstigator, UHealthComponent* OwningComponent, float
						 UpdatedHealth, float Delta);
};
