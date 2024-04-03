#pragma once
#include "AutomationFoundation/Inventory/ItemSpecification.h"

#include "ResourceNode.generated.h"

class UMountComponent;

UCLASS()
class AResourceNode : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UMountComponent* ExtractorMountPoint;

	UPROPERTY(EditDefaultsOnly)
	UItemSpecification* HarvestedResource;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* VeinMesh;

	// Optional TODO: Provide a MAX size of the resource to limit how much can be extracted from a give node
	// Maybe provide an Unlimited option?
	// For now, stick with unlimited...

public:
	AResourceNode();

	UItemSpecification* GetHarvestedResource() const;
};
