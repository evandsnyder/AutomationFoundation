// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationFoundationCharacter.h"

#include "AutomationFoundationGameMode.h"
#include "AutomationFoundationPlayerController.h"
#include "AutomationFoundationUtilities.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Animation/AnimInstance.h"
#include "AutomationFoundation/BuildSystem/BuildingSpecification.h"
#include "AutomationFoundation/BuildSystem/BuildShelfComponent.h"
#include "AutomationFoundation/BuildSystem/PreviewObjects/AFBeltSupportPreview.h"
#include "AutomationFoundation/BuildSystem/PreviewObjects/AFConveyorBeltPreview.h"
#include "AutomationFoundation/BuildSystem/PreviewObjects/AFDefaultPreview.h"
#include "AutomationFoundation/Components/ToolbarComponent.h"
#include "AutomationFoundation/Interaction/IInteractable.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AAutomationFoundationCharacter

AAutomationFoundationCharacter::AAutomationFoundationCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera01"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(20.f, 0.f, 60.f)); // Position the camera
	FirstPersonCamera->bUsePawnControlRotation = true;

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	// HeadMesh->SetOwnerNoSee(true);
	HeadMesh->SetupAttachment(GetCapsuleComponent());
	HeadMesh->SetRelativeLocation(FVector(-30.0f, 0.0f, -150.f));
	HeadMesh->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	// BodyMesh->SetOwnerNoSee(false);
	BodyMesh->SetupAttachment(HeadMesh);

	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	// LegMesh->SetOwnerNoSee(true);
	LegMesh->SetupAttachment(HeadMesh);

	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetMesh"));
	// FeetMesh->SetOwnerNoSee(true);
	FeetMesh->SetupAttachment(HeadMesh);


	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.0f);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	ToolbarComponent = CreateDefaultSubobject<UToolbarComponent>(TEXT("Toolbar"));

	BuildShelf = CreateDefaultSubobject<UBuildShelfComponent>(TEXT("BuildShelf"));
}

void AAutomationFoundationCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (AAutomationFoundationPlayerController* PlayerController = Cast<AAutomationFoundationPlayerController>(Controller))
	{
		InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		verify(InputSubsystem.IsValid())
		InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AAutomationFoundationCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// TODO: Add Jumping Mechanic
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::StopJump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAutomationFoundationCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAutomationFoundationCharacter::Look);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::Interact);
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::OpenInventory);

		// Bind Toolbar Actions
		EnhancedInputComponent->BindAction(ToolBarQuickSlotActionZero, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::QuickActionZero);
		EnhancedInputComponent->BindAction(ToolBarQuickSlotActionOne, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::QuickActionOne);
		EnhancedInputComponent->BindAction(ToolBarQuickSlotActionTwo, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::QuickActionTwo);
		EnhancedInputComponent->BindAction(ToolBarQuickSlotActionThree, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::QuickActionThree);
		EnhancedInputComponent->BindAction(ToolBarQuickSlotActionFour, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::QuickActionFour);
		EnhancedInputComponent->BindAction(ToolBarQuickSlotActionFive, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::QuickActionFive);
		EnhancedInputComponent->BindAction(ToolBarQuickSlotActionSix, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::QuickActionSix);
		EnhancedInputComponent->BindAction(ToolBarQuickSlotActionSeven, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::QuickActionSeven);
		EnhancedInputComponent->BindAction(ToolBarQuickSlotActionEight, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::QuickActionEight);
		EnhancedInputComponent->BindAction(ToolBarQuickSlotActionNine, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::QuickActionNine);

		// Bind Build ModeActions
		EnhancedInputComponent->BindAction(ToggleBuildModeAction, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::ToggleBuildMode);
		EnhancedInputComponent->BindAction(RotateItemAction, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::RotatePlaceableItem);
		EnhancedInputComponent->BindAction(PlaceItemAction, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::PlaceItem);
		EnhancedInputComponent->BindAction(CustomScrollAction, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::CustomScroll);

		// Bind BuildShelf Actions
		EnhancedInputComponent->BindAction(BuildToolBarQuickSlotActionZero, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::BuildQuickActionZero);
		EnhancedInputComponent->BindAction(BuildToolBarQuickSlotActionOne, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::BuildQuickActionOne);
		EnhancedInputComponent->BindAction(BuildToolBarQuickSlotActionTwo, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::BuildQuickActionTwo);
		EnhancedInputComponent->BindAction(BuildToolBarQuickSlotActionThree, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::BuildQuickActionThree);
		EnhancedInputComponent->BindAction(BuildToolBarQuickSlotActionFour, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::BuildQuickActionFour);
		EnhancedInputComponent->BindAction(BuildToolBarQuickSlotActionFive, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::BuildQuickActionFive);
		EnhancedInputComponent->BindAction(BuildToolBarQuickSlotActionSix, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::BuildQuickActionSix);
		EnhancedInputComponent->BindAction(BuildToolBarQuickSlotActionSeven, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::BuildQuickActionSeven);
		EnhancedInputComponent->BindAction(BuildToolBarQuickSlotActionEight, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::BuildQuickActionEight);
		EnhancedInputComponent->BindAction(BuildToolBarQuickSlotActionNine, ETriggerEvent::Started, this, &AAutomationFoundationCharacter::BuildQuickActionNine);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void AAutomationFoundationCharacter::OnEnterActor(TObjectPtr<AActor> InteractiveActor)
{
	CurrentInteractable = Cast<IInteractable>(InteractiveActor);
	if (CurrentInteractable)
	{
		CurrentInteractableActor = InteractiveActor;
	}
}


void AAutomationFoundationCharacter::OnLeaveActor()
{
	CurrentInteractable = nullptr;
	CurrentInteractableActor = nullptr;
}

bool AAutomationFoundationCharacter::AddItemToInventory(TObjectPtr<UInventoryItemInstance> NewItem) const
{
	check(InventoryComponent != nullptr);
	return InventoryComponent->TryAddItemToInventory(NewItem);
}

void AAutomationFoundationCharacter::ToggleBuildMode(const FInputActionValue& Value)
{
	switch (PlayerMode)
	{
	case EPlayerMode::Default:
		EnterBuildMode();
		OnPlayerModeChanged.Broadcast(EPlayerMode::Build);
		break;
	case EPlayerMode::Build:
		ExitBuildMode();
		OnPlayerModeChanged.Broadcast(EPlayerMode::Default);
		break;
	default:
		checkNoEntry();
	}
}

void AAutomationFoundationCharacter::RotatePlaceableItem(const FInputActionValue& Value)
{
	if (PlaceableActor)
	{
		PlaceableActor->ScrollPreview(Value.GetMagnitude());
		// TODO: Why is this the Y Axis ???
		// const FRotator NewRotation = PlaceableActor->GetActorRotation() + FRotator(0.0f, 45.0f * Value.GetMagnitude(), 0.0f);
		// PlaceableActor->SetActorRotation(NewRotation.Clamp());
	}
}

void AAutomationFoundationCharacter::PlaceItem(const FInputActionValue& Value)
{
	// TODO: The Placeable should be responsible for all placement logic

	if (IsValid(PlaceableActor))
	{
		if (PlaceableActor->PlacePreview())
		{
			// TODO: if the placement succeeds, then we need to accept the cost of the placement
			LOG_INFO(LogTemp, "Placed New Item in the world");
		}
	}
}

void AAutomationFoundationCharacter::CustomScroll(const FInputActionValue& Value)
{
	if (IsValid(PlaceableActor))
	{
		PlaceableActor->CustomScrollPreview(Value.GetMagnitude());
	}
}

bool AAutomationFoundationCharacter::TryAddItemToInventory(UInventoryItemInstance* NewItem) const
{
	// Try Add to Toolbar first
	bool bResult = ToolbarComponent->TryAddItemToInventory(NewItem);
	if (!bResult)
	{
		bResult = InventoryComponent->TryAddItemToInventory(NewItem);
	}

	return bResult;
}

void AAutomationFoundationCharacter::EnterBuildMode()
{
	LOG_INFO(LogTemp, "Entering Build Mode");
	PlayerMode = EPlayerMode::Build;
	InputSubsystem->AddMappingContext(BuildModeMappingContext, 10); // Higher priority than the default context...

	RefreshBuildItem(0);
}

void AAutomationFoundationCharacter::ExitBuildMode()
{
	LOG_INFO(LogTemp, "Exiting Build Mode");
	PlayerMode = EPlayerMode::Default;
	InputSubsystem->RemoveMappingContext(BuildModeMappingContext);
}

void AAutomationFoundationCharacter::RefreshBuildLocation()
{
	if (PlaceableActor)
	{
		PlaceableActor->UpdateLocation(FindBuildModeLocation());
	}
}

void AAutomationFoundationCharacter::RefreshBuildItem(int32 ActiveToolbarSlot)
{
	if (IsValid(PlaceableActor))
	{
		PlaceableActor->CancelPreview();
	}

	if (BuildLocationRefreshTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(BuildLocationRefreshTimer);
	}

	ActiveToolbarSlot -= 1;
	if (ActiveToolbarSlot < 0)
	{
		// Safety check to loop back to the end of the slot index stuff..
		ActiveToolbarSlot = 9;
	}
	UBuildingSpecification* BuildingSpecification = BuildShelf->GetBuildModelByIndex(ActiveToolbarSlot);
	if (!IsValid(BuildingSpecification))
	{
		// Slot is empty or a nullptr 
		return;
	}

	LOG_INFO(LogTemp, "Set Build component to : %s", *(BuildingSpecification->BuildingID.ToString()));

	const TSubclassOf<AActor> PreviewClass = BuildingSpecification->PreviewClass;

	const FVector SpawnLocation;
	const FRotator SpawnRotation;
	PlaceableActor = GetWorld()->SpawnActor<AAFPreview>(PreviewClass, SpawnLocation, SpawnRotation);
	if (PlaceableActor)
	{
		PlaceableActor->Configure(BuildingSpecification);
		PlaceableActor->CreatePreview();

		GetWorldTimerManager().SetTimer(BuildLocationRefreshTimer, this, &AAutomationFoundationCharacter::RefreshBuildLocation, 0.1f,
		                                true);
	}
}

FHitResult AAutomationFoundationCharacter::FindBuildModeLocation() const
{
	// Raycast straight out
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(TEXT("BuildLineTrace"), true, this);
	TraceParameters.bDebugQuery = true;

	if (IsValid(PlaceableActor))
	{
		TraceParameters.AddIgnoredActor(PlaceableActor);
	}

	TraceParameters.bTraceComplex = true;
	TraceParameters.bReturnPhysicalMaterial = false;

	FVector InitialTraceStart = FirstPersonCamera->GetComponentLocation();
	FVector InitialTraceEnd = InitialTraceStart + (FirstPersonCamera->GetForwardVector() * 500.f);
	FHitResult HitResult;

	DrawDebugLine(GetWorld(), InitialTraceStart, InitialTraceEnd, FColor::Red);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, InitialTraceStart, InitialTraceEnd, ECC_Visibility, TraceParameters))
	{
		return HitResult;
	}

	// Run a second trace down into the ground...
	// Start now starts at the trace end
	FVector SecondTraceEnd = InitialTraceEnd - FVector(0.f, 0.f, 500.f);

	DrawDebugLine(GetWorld(), InitialTraceEnd, SecondTraceEnd, FColor::Green); // Cast straight down...
	GetWorld()->LineTraceSingleByChannel(HitResult, InitialTraceEnd, SecondTraceEnd, ECC_Visibility, TraceParameters);
	return HitResult;
}

void AAutomationFoundationCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AAutomationFoundationCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAutomationFoundationCharacter::Interact(const FInputActionValue& Value)
{
	if (CurrentInteractable)
	{
		CurrentInteractable->OnInteract();
	}
}

void AAutomationFoundationCharacter::OpenInventory(const FInputActionValue& Value)
{
	UE_LOG(LogTemplateCharacter, Display, TEXT("Opening Inventory"));
	ToggleInventoryUI();
}

void AAutomationFoundationCharacter::QuickActionZero(const FInputActionValue& Value)
{
	ToolbarComponent->SetActiveSlot(0);
}

void AAutomationFoundationCharacter::QuickActionOne(const FInputActionValue& Value)
{
	ToolbarComponent->SetActiveSlot(1);
}

void AAutomationFoundationCharacter::QuickActionTwo(const FInputActionValue& Value)
{
	ToolbarComponent->SetActiveSlot(2);
}

void AAutomationFoundationCharacter::QuickActionThree(const FInputActionValue& Value)
{
	ToolbarComponent->SetActiveSlot(3);
}

void AAutomationFoundationCharacter::QuickActionFour(const FInputActionValue& Value)
{
	ToolbarComponent->SetActiveSlot(4);
}

void AAutomationFoundationCharacter::QuickActionFive(const FInputActionValue& Value)
{
	ToolbarComponent->SetActiveSlot(5);
}

void AAutomationFoundationCharacter::QuickActionSix(const FInputActionValue& Value)
{
	ToolbarComponent->SetActiveSlot(6);
}

void AAutomationFoundationCharacter::QuickActionSeven(const FInputActionValue& Value)
{
	ToolbarComponent->SetActiveSlot(7);
}

void AAutomationFoundationCharacter::QuickActionEight(const FInputActionValue& Value)
{
	ToolbarComponent->SetActiveSlot(8);
}

void AAutomationFoundationCharacter::QuickActionNine(const FInputActionValue& Value)
{
	ToolbarComponent->SetActiveSlot(9);
}

void AAutomationFoundationCharacter::BuildQuickActionZero(const FInputActionValue& Value)
{
	RefreshBuildItem(0);
}

void AAutomationFoundationCharacter::BuildQuickActionOne(const FInputActionValue& Value)
{
	RefreshBuildItem(1);
}

void AAutomationFoundationCharacter::BuildQuickActionTwo(const FInputActionValue& Value)
{
	RefreshBuildItem(2);
}

void AAutomationFoundationCharacter::BuildQuickActionThree(const FInputActionValue& Value)
{
	RefreshBuildItem(3);
}

void AAutomationFoundationCharacter::BuildQuickActionFour(const FInputActionValue& Value)
{
	RefreshBuildItem(4);
}

void AAutomationFoundationCharacter::BuildQuickActionFive(const FInputActionValue& Value)
{
	RefreshBuildItem(5);
}

void AAutomationFoundationCharacter::BuildQuickActionSix(const FInputActionValue& Value)
{
	RefreshBuildItem(6);
}

void AAutomationFoundationCharacter::BuildQuickActionSeven(const FInputActionValue& Value)
{
	RefreshBuildItem(7);
}

void AAutomationFoundationCharacter::BuildQuickActionEight(const FInputActionValue& Value)
{
	RefreshBuildItem(8);
}

void AAutomationFoundationCharacter::BuildQuickActionNine(const FInputActionValue& Value)
{
	RefreshBuildItem(9);
}

void AAutomationFoundationCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
