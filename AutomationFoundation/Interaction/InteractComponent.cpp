#include "InteractComponent.h"

#include "EnhancedInputComponent.h"
#include "ICanInteract.h"
#include "IInteract.h"
#include "AutomationFoundation/AutomationFoundation.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/UserInterface/AFUWInteractPrompt.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraComponent = GetOwner()->GetComponentByClass<UCameraComponent>();
	if (!IsValid(CameraComponent))
	{
		LOG_WARNING(LogTemp, "Owning Component Does not have a Camera Component")
		return;
	}

	EnablePromptScanning();

	// Add to viewport..
	if (IsValid(InteractPromptClass))
	{
		InteractPrompt = Cast<UAFUWInteractPrompt>(CreateWidget(GetWorld(), InteractPromptClass));
		InteractPrompt->AddToViewport();
		InteractPrompt->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		LOG_ERROR(LogTemp, "Invalid Interact Prompt");
	}
}

void UInteractComponent::LinkInputMappingContext(UEnhancedInputComponent* InputComponent)
{
	InputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &UInteractComponent::TryInteract);
}

void UInteractComponent::CanInteractTrace()
{
	if (!bShouldScan) return;
	// Run a line trace from here...
	// Get The Actors and the camera's forward position...
	check(IsValid(CameraComponent));

	FCollisionQueryParams QueryParams = FCollisionQueryParams(TEXT("Interact Line Trace"), true, GetOwner());

	FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + (CameraComponent->GetForwardVector() * InteractRange);

	// Draw a Debug line...
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_AF_Interact, QueryParams))
	{
		if (ICanInteract* CanInteract = Cast<ICanInteract>(HitResult.GetActor()))
		{
			if (CanInteract->CanInteract())
			{
				if (Cast<IInteract>(CanInteract))
				{
					CurrentInteractTarget = HitResult.GetActor();
				}
				else
				{
					LOG_ERROR(LogTemp, "Class Implements ICanInteract but not IInteract");
					return;
				}

				TryShowInteractPrompt();
			}
		}
	}
	else
	{
		// Not pointing at anything!
		CurrentInteractTarget.Reset();
		TryHideInteractPrompt();
	}
}

void UInteractComponent::TryInteract(const FInputActionValue& Value)
{
	if (CurrentInteractTarget.IsValid())
	{
		IInteract::Execute_Interact(CurrentInteractTarget.Get(), GetOwner());
	}
}

AActor* UInteractComponent::GetInteractTarget()
{
	return Cast<AActor>(CurrentInteractTarget);
}

void UInteractComponent::ToggleInteractPrompt()
{
	auto InteractVisibility = InteractPrompt->GetVisibility();
	LOG_INFO(LogTemp, "Interact Prompt Visibility: %hhd", InteractVisibility);
	if (InteractPrompt->IsVisible())
	{
		TryHideInteractPrompt();
		// Disable scanning
		DisablePromptScanning();
	}
	else
	{
		EnablePromptScanning();
		// Shouldn't need this yet...
		// TryShowInteractPrompt();
	}
}

void UInteractComponent::EnablePromptScanning()
{
	bShouldScan = true;
	GetWorld()->GetTimerManager().SetTimer(InteractTimer, this, &UInteractComponent::CanInteractTrace, InteractRefreshInterval, true);
}

void UInteractComponent::DisablePromptScanning()
{
	bShouldScan = false;
	GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
}

void UInteractComponent::TryShowInteractPrompt() const
{
	if (!bShouldScan) return;
	ensure(IsValid(InteractPrompt));
	ICanInteract* CanInteract = Cast<ICanInteract>(CurrentInteractTarget);
	if (CanInteract == nullptr) return;
	if (!InteractPrompt->IsVisible())
	{
		InteractPrompt->UpdateInteractText(FText::FromString(CanInteract->GetInteractText()));
		InteractPrompt->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInteractComponent::TryHideInteractPrompt() const
{
	ensure(IsValid(InteractPrompt));
	if (InteractPrompt->IsVisible())
	{
		InteractPrompt->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInteractComponent::TryPlayInteractSound() const
{
	if (ICanInteract* CanInteract = Cast<ICanInteract>(CurrentInteractTarget))
	{
		if (USoundCue* SoundCue = CanInteract->GetInteractSound())
		{
			// Should this be location based??
			UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);
		}
	}
}

void UInteractComponent::TrayPlayInteractAnimation() const
{
	if (ICanInteract* CanInteract = Cast<ICanInteract>(CurrentInteractTarget))
	{
		if (UAnimMontage* AnimMontage = CanInteract->GetInteractAnimation())
		{
			// TODO: Add to Anim BP... etc...
		}
	}
}
