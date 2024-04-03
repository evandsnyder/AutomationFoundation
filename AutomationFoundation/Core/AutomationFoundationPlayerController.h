// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMode.h"
#include "GameFramework/PlayerController.h"
// #include "Inventory\InventoryItem.h"
#include "AutomationFoundationPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 *
 */
UCLASS()
class AUTOMATIONFOUNDATION_API AAutomationFoundationPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	AAutomationFoundationPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenPlayerInventory(AActor* InteractableActor);


protected:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;


	virtual void SetupInputComponent() override;

	// Begin Actor interface
	virtual void BeginPlay() override;
	// End Actor interface
	
};
