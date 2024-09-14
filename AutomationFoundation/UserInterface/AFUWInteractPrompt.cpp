#include "AFUWInteractPrompt.h"

#include "AutomationFoundation/Core/UserInterface/AFTextBlock.h"

void UAFUWInteractPrompt::UpdateButtonIcon(UAFImage* NewIcon)
{
	ButtonIcon = NewIcon;
}

void UAFUWInteractPrompt::UpdateInteractText(const FText& NewText)
{
	InteractLabel->SetText(NewText);
}
