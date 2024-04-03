#pragma once

#include "CoreMinimal.h"
#include "CraftingMachineType.h"
#include "RecipeSpecification.generated.h"

USTRUCT(BlueprintType)
struct FRecipeSpecification : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RecipeID = "UNKNOWN";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText RecipeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText RecipeDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, int32> InputItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, int32> OutputItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CraftingTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanCraftInPlayerInventory = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECraftingMachineType CraftingMachineType = ECraftingMachineType::Basic; // Maybe make a set?

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	// Should there be some kind of recipe class to support alternative recipes??
	// This is a Satisfactory Feature, but I think is too complicated for an MVP
};
