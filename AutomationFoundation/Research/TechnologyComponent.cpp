#include "TechnologyComponent.h"

void UTechnologyComponent::IncrementResearchProgress()
{
	OnResearchProgressIncremented.Broadcast();

	// if progress is now completed
	CompleteCurrentResearch();
}

void UTechnologyComponent::ChangeResearch()
{
	OnResearchChanged.Broadcast();
}

void UTechnologyComponent::CompleteCurrentResearch()
{
	OnResearchCompleted.Broadcast();
	// CurrentResearch = nullptr; // Need to make sure we aren't going to attempt to progress the already completed research
}
