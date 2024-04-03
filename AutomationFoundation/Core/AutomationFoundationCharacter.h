// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PlayerMode.h"
#include "AutomationFoundation/BuildSystem/AFMountedPlaceable.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AutomationFoundationCharacter.generated.h"

class UBuildShelfComponent;
class AAFPreview;
class UEnhancedInputLocalPlayerSubsystem;
class UCameraComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UInputAction;
class UInputMappingContext;
class UInventoryComponent;
class IInteractable;
class USphereComponent;
class UToolbarComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerModeChanged, EPlayerMode, NewMode);

UCLASS(config=Game)
class AAutomationFoundationCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* HeadMesh;
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* LegMesh;
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* FeetMesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollectionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess ="true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UToolbarComponent> ToolbarComponent;


	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	UInputMappingContext* BuildModeMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* OpenInventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToolBarQuickSlotActionZero;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToolBarQuickSlotActionOne;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToolBarQuickSlotActionTwo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToolBarQuickSlotActionThree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToolBarQuickSlotActionFour;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToolBarQuickSlotActionFive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToolBarQuickSlotActionSix;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToolBarQuickSlotActionSeven;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToolBarQuickSlotActionEight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToolBarQuickSlotActionNine;
	// Mouse scroll as well???

	// Build Mode Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode", meta=(AllowPrivateAccess = "true"))
	UInputAction* ToggleBuildModeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode", meta=(AllowPrivateAccess = "true"))
	UInputAction* RotateItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode", meta=(AllowPrivateAccess = "true"))
	UInputAction* PlaceItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode", meta=(AllowPrivateAccess = "true"))
	UInputAction* CustomScrollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildToolBarQuickSlotActionZero;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildToolBarQuickSlotActionOne;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildToolBarQuickSlotActionTwo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildToolBarQuickSlotActionThree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildToolBarQuickSlotActionFour;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildToolBarQuickSlotActionFive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildToolBarQuickSlotActionSix;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildToolBarQuickSlotActionSeven;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildToolBarQuickSlotActionEight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar", meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildToolBarQuickSlotActionNine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build Mode", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UBuildShelfComponent> BuildShelf;

	TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem;
	EPlayerMode PlayerMode = EPlayerMode::Default;

	UPROPERTY()
	TObjectPtr<AAFPreview> PlaceableActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Build Mode")
	TSubclassOf<AAFPlaceablePreview> PlaceableClass;
	UPROPERTY(EditDefaultsOnly, Category="Build Mode")
	TSubclassOf<AAFMountedPlaceable> MountedPlaceableClass;

	FTimerHandle BuildLocationRefreshTimer;

	IInteractable* CurrentInteractable;

public:
	AAutomationFoundationCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> CurrentInteractableActor;

	FOnPlayerModeChanged OnPlayerModeChanged;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);
	void OpenInventory(const FInputActionValue& Value);

	// Toolbar actions
	void QuickActionZero(const FInputActionValue& Value);
	void QuickActionOne(const FInputActionValue& Value);
	void QuickActionTwo(const FInputActionValue& Value);
	void QuickActionThree(const FInputActionValue& Value);
	void QuickActionFour(const FInputActionValue& Value);
	void QuickActionFive(const FInputActionValue& Value);
	void QuickActionSix(const FInputActionValue& Value);
	void QuickActionSeven(const FInputActionValue& Value);
	void QuickActionEight(const FInputActionValue& Value);
	void QuickActionNine(const FInputActionValue& Value);

	void ToggleBuildMode(const FInputActionValue& Value);
	void RotatePlaceableItem(const FInputActionValue& Value);
	void PlaceItem(const FInputActionValue& Value);
	void CustomScroll(const FInputActionValue& Value);

	void BuildQuickActionZero(const FInputActionValue& Value);
	void BuildQuickActionOne(const FInputActionValue& Value);
	void BuildQuickActionTwo(const FInputActionValue& Value);
	void BuildQuickActionThree(const FInputActionValue& Value);
	void BuildQuickActionFour(const FInputActionValue& Value);
	void BuildQuickActionFive(const FInputActionValue& Value);
	void BuildQuickActionSix(const FInputActionValue& Value);
	void BuildQuickActionSeven(const FInputActionValue& Value);
	void BuildQuickActionEight(const FInputActionValue& Value);
	void BuildQuickActionNine(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interfaced

public:
	void OnEnterActor(TObjectPtr<AActor> InteractiveActor);
	void OnLeaveActor();

	bool AddItemToInventory(TObjectPtr<class UInventoryItemInstance> NewItem) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void ShowInteractPrompt();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void HideInteractPrompt();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void ToggleInventoryUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void ToggleCraftingMachineUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void ToggleExtractionMachineUI();

	UFUNCTION()
	bool TryAddItemToInventory(UInventoryItemInstance* NewItem) const;

	UFUNCTION()
	void EnterBuildMode();
	UFUNCTION()
	void ExitBuildMode();

	void RefreshBuildLocation();
	
	UFUNCTION()
	void RefreshBuildItem(int32 ActiveToolbarSlot);

private:
	FHitResult FindBuildModeLocation() const;
};
