// Copyright Epic Games, Inc. All Rights Reserved.


#include "AutomationFoundationPlayerController.h"

#include "AutomationFoundationUtilities.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Cheats/AFCheatManager.h"

AAutomationFoundationPlayerController::AAutomationFoundationPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CheatClass = UAFCheatManager::StaticClass();
}

void AAutomationFoundationPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AAutomationFoundationPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}
}
