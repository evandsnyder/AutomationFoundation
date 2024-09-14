#pragma once

#include "InteractComponent.generated.h"

struct FInputActionValue;
class UAFUWInteractPrompt;
class UInputAction;
class IInteract;
class UCameraComponent;

UCLASS()
class UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float InteractRefreshInterval = 0.1f;

	UPROPERTY(EditAnywhere)
	float InteractRange = 150.0f;

	UPROPERTY()
	FTimerHandle InteractTimer;

	UPROPERTY()
	UCameraComponent* CameraComponent;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentInteractTarget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFUWInteractPrompt> InteractPromptClass;

	UPROPERTY()
	UAFUWInteractPrompt* InteractPrompt;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> InteractAction;

public:
	virtual void BeginPlay() override;

	void LinkInputMappingContext(UEnhancedInputComponent* InputComponent);

	UFUNCTION()
	void CanInteractTrace();

	UFUNCTION()
	void TryInteract(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	AActor* GetInteractTarget();

	UFUNCTION(BlueprintCallable)
	void ToggleInteractPrompt();

private:
	void EnablePromptScanning();
	void DisablePromptScanning();

	void TryShowInteractPrompt() const;
	void TryHideInteractPrompt() const;

	void TryPlayInteractSound() const;
	void TrayPlayInteractAnimation() const;

	bool bShouldScan = true;
};
