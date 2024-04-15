#pragma once

#include "Placeable.h"
#include "Machine.generated.h"

class UItemSpecification;
class AConveyorItem;
class UInventoryItemInstance;
class AAttachPoint;

UCLASS()
class AMachine : public APlaceable //, IInteractable
{
	GENERATED_BODY()

protected:
	// Needs to have some combination of inputs and outputs
	// TMap<AAttachPoint*, AConveyorBelt*> AttachPointsToConveyors
	UPROPERTY(BlueprintReadWrite)
	TMap<AAttachPoint*, float> Outputs; // float is output rate..
	UPROPERTY(BlueprintReadWrite)
	TMap<AAttachPoint*, UInventoryItemInstance*> Inputs;

	float ItemSpawnInterval = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AConveyorItem> ConveyorItemClass;

public:
	AMachine();
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	void ConnectAttachPointAsInput(AAttachPoint* AttachPoint);
	UFUNCTION(BlueprintCallable)
	void ConnectAttachPointAsOutput(AAttachPoint* AttachPoint);

	UFUNCTION(BlueprintCallable)
	bool OutputFromAttachPoint(AAttachPoint* OutputAttachPoint, UItemSpecification* ItemSpecification);

	UFUNCTION(BlueprintCallable)
	bool AcceptInput(AAttachPoint* ProvidingAttachPoint, AConveyorItem* IncomingItem);

	UFUNCTION(BlueprintNativeEvent)
	void StartOutputting();
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool CanOutput();

	UFUNCTION()
	bool CanAcceptInput(AAttachPoint* InputAttachPoint, AConveyorItem* IncomingItem);
};
