#pragma once

#include "RecipeSpecification.h"
#include "RecipeSpecificationDataTableReference.generated.h"

USTRUCT(BlueprintType)
struct FRecipeSpecificationDataTableReference : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RecipeID = "UNKNOWN";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<URecipeSpecification> RecipeSpecificationRef;
};
