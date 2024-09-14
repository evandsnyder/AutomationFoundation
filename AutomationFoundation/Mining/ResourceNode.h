#pragma once
#include "AutomationFoundation/Crafting/RecipeSpecification.h"

#include "ResourceNode.generated.h"

class UMountComponent;

UCLASS()
class AResourceNode : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	URecipeSpecification* Recipe;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* VeinMesh;

	// Optional TODO: Provide a MAX size of the resource to limit how much can be extracted from a give node
	// Maybe provide an Unlimited option?
	// For now, stick with unlimited...

public:
	AResourceNode();

	UFUNCTION(BlueprintCallable)
	URecipeSpecification* GetHarvestedResource() const;
};
