#pragma once
#include "Engine/StreamableManager.h"
#include "AutomationFoundationGlobals.generated.h"

UCLASS()
class UAutomationFoundationGlobals : public UObject
{
	GENERATED_BODY()

	static UAutomationFoundationGlobals* Instance;

public:
	static FStreamableManager StreamableManager;

	static UAutomationFoundationGlobals* Get()
	{
		if(!Instance)
		{
			Instance = NewObject<UAutomationFoundationGlobals>();
		}

		return Instance;
	}
};
