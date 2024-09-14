// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationFoundationGameMode.h"

#include "AutomationFoundation/BuildSystem/BuildingSpecificationDataTableReference.h"
#include "AutomationFoundation/Crafting/RecipeSpecificationDataTableReference.h"
#include "UObject/ConstructorHelpers.h"

class RecipeSpecificationDataTableReference;

AAutomationFoundationGameMode::AAutomationFoundationGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/AutomationFoundation/Core/BP_CharacterRevamp.BP_CharacterRevamp"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> RecipeDataBaseFinder(
		TEXT("/Script/Engine.DataTable'/Game/AutomationFoundation/Data/DT_RecipeDatabase.DT_RecipeDatabase'"));
	RecipeDatabase = RecipeDataBaseFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataBaseFinder(
		TEXT("/Script/Engine.DataTable'/Game/AutomationFoundation/Data/DT_ItemDatabase.DT_ItemDatabase'"));
	ItemDatabase = ItemDataBaseFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> BuildingDataBaseFinder(
		TEXT("/Script/Engine.DataTable'/Game/AutomationFoundation/Data/DT_BuildingDatabase.DT_BuildingDatabase'"));
	BuildingDatabase = BuildingDataBaseFinder.Object;
}

bool AAutomationFoundationGameMode::FindItem(const FName& ItemID, FItemSpecificationDataTableReference& InItem)
{
	// ItemID should be same as RowName
	if (const FItemSpecificationDataTableReference* ResultPtr = ItemDatabase->FindRow<FItemSpecificationDataTableReference>(ItemID, ""); ResultPtr != nullptr)
	{
		InItem = *ResultPtr;
		return true;
	}

	return false;
}

bool AAutomationFoundationGameMode::FindRecipe_Implementation(FName RecipeID, FRecipeSpecificationDataTableReference& InRecipe)
{
	if (const FRecipeSpecificationDataTableReference* ResultPtr = ItemDatabase->FindRow<FRecipeSpecificationDataTableReference>(RecipeID, ""); ResultPtr != nullptr)
	{
		InRecipe = *ResultPtr;
		return true;
	}
	return false;
}

TArray<FItemSpecificationDataTableReference> AAutomationFoundationGameMode::FindAllItems()
{
	TArray<FItemSpecificationDataTableReference> Result;
	ItemDatabase->ForeachRow<FItemSpecificationDataTableReference>(
		"", [&](const FName& RowName, const FItemSpecificationDataTableReference& ItemSpecification)
		{
			Result.Add(ItemSpecification);
		});

	return Result;
}

TArray<URecipeSpecification*> AAutomationFoundationGameMode::FindAllRecipes(
	ECraftingMachineFilterType MachineFilterType)
{
	TArray<URecipeSpecification*> Result;
	ECraftingMachineType FilterType = ConvertFilterToType(MachineFilterType);
	// Let's get all recipes and then filter..
	RecipeDatabase->ForeachRow<FRecipeSpecificationDataTableReference>(
		"", [&](const FName& RowName, const FRecipeSpecificationDataTableReference& Specification)
		{
			URecipeSpecification* Recipe = Specification.RecipeSpecificationRef.LoadSynchronous();
			if (MachineFilterType == ECraftingMachineFilterType::InventoryOnly && Recipe->bCanCraftInPlayerInventory)
			{
				Result.Add(Recipe);
			}
			if (MachineFilterType == ECraftingMachineFilterType::All || Recipe->CraftingMachineType == FilterType)
			{
				Result.Add(Recipe);
			}
		});

	return Result;
}

TArray<UBuildingSpecification*> AAutomationFoundationGameMode::FindAllBuildings() const
{
	if (!IsValid(BuildingDatabase)) return {};

	TArray<UBuildingSpecification*> Buildings;

	BuildingDatabase->ForeachRow<FBuildingSpecificationDataTableReference>("", [&](const FName& RowName, const FBuildingSpecificationDataTableReference& Specification)
	{
		Buildings.Add(Specification.BuildingSpecificationRef.LoadSynchronous());
	});

	return Buildings;
}

ECraftingMachineType AAutomationFoundationGameMode::ConvertFilterToType(ECraftingMachineFilterType FilterType)
{
	switch (FilterType)
	{
	case ECraftingMachineFilterType::All:
		return ECraftingMachineType::Unknown;
	case ECraftingMachineFilterType::Basic:
		return ECraftingMachineType::Basic;
	case ECraftingMachineFilterType::Chemical:
		return ECraftingMachineType::Chemical;
	case ECraftingMachineFilterType::ResourceExtractor:
		return ECraftingMachineType::ResourceExtractor;
	case ECraftingMachineFilterType::Smelter:
		return ECraftingMachineType::Smelter;
	case ECraftingMachineFilterType::InventoryOnly:
		return ECraftingMachineType::Unknown;
	}

	return ECraftingMachineType::Unknown;
}
