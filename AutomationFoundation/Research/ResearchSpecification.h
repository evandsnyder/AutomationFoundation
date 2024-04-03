#pragma once

#include "CoreMinimal.h"
#include "AutomationFoundation/Inventory/ItemSpecification.h"
#include "ResearchSpecification.generated.h"

USTRUCT()
struct FResearchSpecification
{
	GENERATED_USTRUCT_BODY()

	// Required Ingredients..
	// UPROPERTY(EditDefaultsOnly)
	// TMap<FItemSpecification, int32> RequiredIngredients;

	// UPROPERTY(EditDefaultsOnly)
	// TArray<FResearchSpecification*> PrerequisiteResearches;

	// And What does this unlock for us?
	// This might be additional researches?
	// This may be New Recipes
	// This may be modifications to performance (i.e, machines are 10% more efficient??
	
};
