// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PlayerMode.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AutomationFoundationCharacter.generated.h"

class UInteractComponent;
class UBuildShelfComponent;
class APlaceable;
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TObjectPtr<UToolbarComponent> ToolbarComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	TObjectPtr<UInteractComponent> InteractComponent;


	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputMappingContext* BuildModeMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* OpenInventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar")
	UInputAction* ToolBarQuickSlotActionZero;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar")
	UInputAction* ToolBarQuickSlotActionOne;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar")
	UInputAction* ToolBarQuickSlotActionTwo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar")
	UInputAction* ToolBarQuickSlotActionThree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar")
	UInputAction* ToolBarQuickSlotActionFour;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar")
	UInputAction* ToolBarQuickSlotActionFive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar")
	UInputAction* ToolBarQuickSlotActionSix;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar")
	UInputAction* ToolBarQuickSlotActionSeven;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar")
	UInputAction* ToolBarQuickSlotActionEight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Toolbar")
	UInputAction* ToolBarQuickSlotActionNine;
	// Mouse scroll as well???

	// Build Mode Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode")
	UInputAction* ToggleBuildModeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode")
	UInputAction* RotateItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode")
	UInputAction* PlaceItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode")
	UInputAction* CustomScrollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar")
	UInputAction* BuildToolBarQuickSlotActionZero;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar")
	UInputAction* BuildToolBarQuickSlotActionOne;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar")
	UInputAction* BuildToolBarQuickSlotActionTwo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar")
	UInputAction* BuildToolBarQuickSlotActionThree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar")
	UInputAction* BuildToolBarQuickSlotActionFour;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar")
	UInputAction* BuildToolBarQuickSlotActionFive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar")
	UInputAction* BuildToolBarQuickSlotActionSix;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar")
	UInputAction* BuildToolBarQuickSlotActionSeven;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar")
	UInputAction* BuildToolBarQuickSlotActionEight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Build Mode|Toolbar")
	UInputAction* BuildToolBarQuickSlotActionNine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build Mode")
	TObjectPtr<UBuildShelfComponent> BuildShelf;

	TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem;
	EPlayerMode PlayerMode = EPlayerMode::Default;

	UPROPERTY()
	TObjectPtr<APlaceable> PlaceableActor = nullptr;

	FTimerHandle BuildLocationRefreshTimer;

public:
	AAutomationFoundationCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> CurrentInteractableActor;

	FOnPlayerModeChanged OnPlayerModeChanged;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

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
	bool AddItemToInventory(TObjectPtr<class UInventoryItemInstance> NewItem) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void ToggleInventoryUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void ToggleCraftingMachineUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void ToggleExtractionMachineUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void ToggleStorageUI();

	UFUNCTION()
	bool TryAddItemToInventory(UInventoryItemInstance* NewItem) const;

	UFUNCTION()
	void EnterBuildMode();
	UFUNCTION()
	void ExitBuildMode();

	void RefreshBuildLocation();

	UFUNCTION()
	void RefreshBuildItem(int32 ActiveToolbarSlot);

	UPROPERTY()
	AActor* EquippedItem;

	UPROPERTY()
	TWeakObjectPtr<UInventoryItemInstance> CurrentItemSlot;

	UFUNCTION()
	void RefreshEquippedItem(int32 NewEquippedItemSlot);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateAnimInstance(EEquipType NewEquipType);
private:
	FHitResult FindBuildModeLocation(ECollisionChannel CollisionChannel = ECC_Visibility) const;
};
