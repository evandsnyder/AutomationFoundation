#include "BuildShelfComponent.h"

#include "AutomationFoundation/Core/AutomationFoundationGameMode.h"

UBuildShelfComponent::UBuildShelfComponent()
{
	BuildShelf.Init(nullptr, 10);
}

void UBuildShelfComponent::BeginPlay()
{
	Super::BeginPlay();
	//We're going to setup the default here
	if (const AAutomationFoundationGameMode* GameMode = Cast<AAutomationFoundationGameMode>(GetWorld()->GetAuthGameMode()))
	{
		TArray<UBuildingSpecification*> Buildings = GameMode->FindAllBuildings();

		int32 Index = 0;
		for (auto Building : Buildings)
		{
			if (Index >= 10) break;
			BuildShelf[Index] = Building;
			Index++;
		}
	}
}

UBuildingSpecification* UBuildShelfComponent::UpdateIndexAndGetBuildModel(int32 Index)
{
	if (Index < 0 || Index > BuildShelf.Num())
	{
		CurrentIndex = 0;
	}

	CurrentIndex = Index;

	return BuildShelf[CurrentIndex];
}

int32 UBuildShelfComponent::GetCurrentIndex() const
{
	return CurrentIndex;
}
