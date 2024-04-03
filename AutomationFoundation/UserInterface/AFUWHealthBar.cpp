#include "AFUWHealthBar.h"

#include "AutomationFoundation/Components/HealthComponent.h"
#include "Components/ProgressBar.h"

void UAFUWHealthBar::OnHealthChanged(AActor* InInstigator, UHealthComponent* OwningComponent, float UpdatedHealth, float Delta)
{
	ProgressBar->SetPercent(OwningComponent->GetHealth() / OwningComponent->GetMaxHealth());
}
