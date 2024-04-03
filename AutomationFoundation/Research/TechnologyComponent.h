#pragma once

#include "CoreMinimal.h"
#include "TechnologyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResearchChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResearchCompletedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResearchProgressIncrementedDelegate);

UCLASS()
class UTechnologyComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Should this class keep track of the progress of ALsL recipes?
	// How will machines indicate when they are done with a crafting cycle???

	// UPROPERTY()
	// TObjectPtr<FResearchSpecification> CurrentResearch;

	UPROPERTY()
	int32 ResearchProgress; //What does research progress look like? This will depend on if all 
	
public:
	FOnResearchChangedDelegate OnResearchChanged;
	FOnResearchCompletedDelegate OnResearchCompleted;
	FOnResearchProgressIncrementedDelegate OnResearchProgressIncremented;
	
	UFUNCTION()
	void IncrementResearchProgress();

	UFUNCTION()
	void ChangeResearch();

	UFUNCTION()
	void CompleteCurrentResearch();
};
