// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationFoundationGameMode.h"

#include "AutomationFoundation/BuildSystem/BuildingSpecificationDataTableReference.h"
#include "UObject/ConstructorHelpers.h"

AAutomationFoundationGameMode::AAutomationFoundationGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/AutomationFoundation/Core/BP_AutomationFoundationCharacter"));
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

FRecipeSpecification AAutomationFoundationGameMode::FindRecipe_Implementation(FName RecipeID, bool& Success)
{
	FRecipeSpecification Result{};
	const FRecipeSpecification* ResultPtr = RecipeDatabase->FindRow<FRecipeSpecification>(RecipeID, "");

	Success = ResultPtr != nullptr;

	if (Success)
		Result = *ResultPtr;

	return Result;
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

TArray<FRecipeSpecification> AAutomationFoundationGameMode::FindAllRecipes(
	ECraftingMachineFilterType MachineFilterType)
{
	TArray<FRecipeSpecification> Result;

	switch (MachineFilterType)
	{
	case ECraftingMachineFilterType::All:
		RecipeDatabase->ForeachRow<FRecipeSpecification>(
			"", [&](const FName& RowName, const FRecipeSpecification& Specification)
			{
				Result.Add(Specification);
			});
		break;
	case ECraftingMachineFilterType::Basic:
		RecipeDatabase->ForeachRow<FRecipeSpecification>(
			"", [&](const FName& RowName, const FRecipeSpecification& Specification)
			{
				if (Specification.CraftingMachineType == ECraftingMachineType::Basic)
				{
					Result.Add(Specification);
				}
			});
		break;
	case ECraftingMachineFilterType::Chemical:
		RecipeDatabase->ForeachRow<FRecipeSpecification>(
			"", [&](const FName& RowName, const FRecipeSpecification& Specification)
			{
				if (Specification.CraftingMachineType == ECraftingMachineType::Chemical)
				{
					Result.Add(Specification);
				}
			});
		break;
	case ECraftingMachineFilterType::ResourceExtractor:
		RecipeDatabase->ForeachRow<FRecipeSpecification>(
			"", [&](const FName& RowName, const FRecipeSpecification& Specification)
			{
				if (Specification.CraftingMachineType == ECraftingMachineType::ResourceExtractor)
				{
					Result.Add(Specification);
				}
			});
		break;
	case ECraftingMachineFilterType::Smelter:
		RecipeDatabase->ForeachRow<FRecipeSpecification>(
			"", [&](const FName& RowName, const FRecipeSpecification& Specification)
			{
				if (Specification.CraftingMachineType == ECraftingMachineType::Smelter)
				{
					Result.Add(Specification);
				}
			});
		break;
	case ECraftingMachineFilterType::InventoryOnly:
		RecipeDatabase->ForeachRow<FRecipeSpecification>(
			"", [&](const FName& RowName, const FRecipeSpecification& Specification)
			{
				if (Specification.bCanCraftInPlayerInventory)
				{
					Result.Add(Specification);
				}
			});
		break;
	}

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
