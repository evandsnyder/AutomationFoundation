#pragma once

#include "ItemSpecification.h"
#include "InventoryItemInstance.generated.h"

UCLASS(BlueprintType)
class UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta=(ExposeOnSpawn=true))
	UItemSpecification* ItemSpecification;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentStackSize = 1;

	UInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	UStaticMesh* GetDisplayMesh() const;

	UFUNCTION()
	bool HasClassificationFlag(EItemClassification ItemClassificationFlag) const;

	UFUNCTION(BlueprintCallable)
	void ModifyCurrentStackSize(int32 Value);
	

private:
	friend class UInventoryComponent;
};
