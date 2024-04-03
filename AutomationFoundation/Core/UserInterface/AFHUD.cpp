#include "AFHUD.h"

#include "AFNotificationPanel.h"
#include "Blueprint/UserWidget.h"
#include "AFUserWidget.h"
#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "Inventory/AFInventory.h"

void AAFHUD::OnPlayerModeChanged(EPlayerMode NewMode)
{
	if (NewMode == EPlayerMode::Build)
	{
		EnterBuildMode();
	}
	else
	{
		ExitBuildMode();
	}
}

void AAFHUD::BeginPlay()
{
	Super::BeginPlay();

	InventoryWidget = CreateWidget<UAFInventoryInterface>(GetOwningPlayerController(), InventoryClass, TEXT("InventoryWidget"));
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

	AllGameWidgets.Add(InventoryWidget);

	ToolbarWidget = CreateWidget<UAFInventory>(GetOwningPlayerController(), ToolbarClass, TEXT("Toolbar Widget"));
	ToolbarWidget->AddToViewport();
	// ToolbarWidget->SetVisibility(ESlateVisibility::Collapsed);

	InventoryNotificationPanel = CreateWidget<UAFNotificationPanel>(GetOwningPlayerController(), InventoryNotificationPanelClass, TEXT("Inventory Notification Panel"));
	InventoryNotificationPanel->AddToViewport();

	if (AAutomationFoundationCharacter* Character = Cast<AAutomationFoundationCharacter>(GetOwningPawn()))
	{
		Character->OnPlayerModeChanged.AddDynamic(this, &AAFHUD::OnPlayerModeChanged);
	}
}

void AAFHUD::ToggleWidget(UAFUserWidget* NewFocusTarget)
{
	check(IsValid(NewFocusTarget));
	FocusTarget = NewFocusTarget;

	for (UAFUserWidget* CurrentWidget : AllGameWidgets)
	{
		if (CurrentWidget != FocusTarget)
		{
			if (CurrentWidget->IsVisible())
			{
				// Hide all other non-target widgets
				CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			continue;
		}

		if (FocusTarget->IsVisible())
		{
			FocusGame();
		}
		else
		{
			FocusWidget();
		}
	}
}

void AAFHUD::ToggleCharacterInventory()
{
	ToggleWidget(InventoryWidget);
	InventoryWidget->OnOpenPlayerInventory();
}

void AAFHUD::ToggleCraftingMachine()
{
	ToggleWidget(InventoryWidget);
	InventoryWidget->OnOpenCraftingMachine();
}

void AAFHUD::ToggleExtractionMachine()
{
	ToggleWidget(InventoryWidget);
	InventoryWidget->OnOpenExtractionMachine();
}

void AAFHUD::EnterBuildMode()
{
	if (!IsValid(BuildToolbar))
	{
		BuildToolbar = CreateWidget<UAFUserWidget>(GetOwningPlayerController(), BuildToolbarClass, TEXT("Build Toolbar"));
		BuildToolbar->AddToViewport();
	}
	ToolbarWidget->SetVisibility(ESlateVisibility::Collapsed);
	BuildToolbar->SetVisibility(ESlateVisibility::Visible);
}

void AAFHUD::ExitBuildMode()
{
	ToolbarWidget->SetVisibility(ESlateVisibility::Visible);
	BuildToolbar->SetVisibility(ESlateVisibility::Collapsed);
}

UAFUserWidget* AAFHUD::InstantiateWidget(const TSubclassOf<UAFUserWidget> WidgetClass) const
{
	UAFUserWidget* NewWidget = CreateWidget<UAFUserWidget>(GetOwningPlayerController(), WidgetClass);
	NewWidget->AddToViewport();
	NewWidget->SetVisibility(ESlateVisibility::Collapsed);
	return NewWidget;
}


void AAFHUD::FocusGame() const
{
	FocusTarget->SetVisibility(ESlateVisibility::Collapsed);
	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
	}
}

void AAFHUD::FocusWidget() const
{
	FocusTarget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->SetShowMouseCursor(true);
	}

	FocusTarget->WidgetActivated();
}
