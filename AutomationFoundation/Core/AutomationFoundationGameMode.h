// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AutomationFoundation/BuildSystem/BuildingSpecification.h"
#include "AutomationFoundation/Crafting/CraftingMachineType.h"
#include "AutomationFoundation/Crafting/RecipeSpecification.h"
#include "AutomationFoundation/Crafting/RecipeSpecificationDataTableReference.h"
#include "AutomationFoundation/Inventory/ItemSpecificationDataTableReference.h"
#include "GameFramework/GameModeBase.h"
#include "AutomationFoundationGameMode.generated.h"

UCLASS(minimalapi)
class AAutomationFoundationGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item System")
	UDataTable* ItemDatabase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting System")
	TObjectPtr<UDataTable> RecipeDatabase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Build System")
	UDataTable* BuildingDatabase;

public:
	AAutomationFoundationGameMode();

	UFUNCTION(BlueprintCallable, Category = "Item System")
	bool FindItem(const FName& ItemID, FItemSpecificationDataTableReference& InItem);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Crafting System")
	bool FindRecipe(FName RecipeID, FRecipeSpecificationDataTableReference& InRecipe);

	UFUNCTION(BlueprintCallable, Category="Item System")
	TArray<FItemSpecificationDataTableReference> FindAllItems(); // Maybe with a filter??

	UFUNCTION(BlueprintCallable, Category="Item System")
	TArray<URecipeSpecification*> FindAllRecipes(ECraftingMachineFilterType MachineType = ECraftingMachineFilterType::All);

	UFUNCTION(BlueprintCallable, Category = "Build System")
	TArray<UBuildingSpecification*> FindAllBuildings() const;

	UFUNCTION(BlueprintCallable)
	ECraftingMachineType ConvertFilterToType(ECraftingMachineFilterType FilterType);
};
