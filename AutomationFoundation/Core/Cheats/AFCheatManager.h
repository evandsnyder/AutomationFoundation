#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "AFCheatManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCheatManager, Log, All);

UCLASS(Within = AutomationFoundationPlayerController )
class UAFCheatManager : public UCheatManager
{
	GENERATED_BODY()
public:
	UFUNCTION(exec)
	void AddItem(const FName& ItemID, int32 Count = 1) const;
};
