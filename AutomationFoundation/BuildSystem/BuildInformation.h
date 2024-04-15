#pragma once

#include "BuildInformation.generated.h"

class APlaceable;
class AAttachPoint;

USTRUCT(BlueprintType)
struct FBuildInformation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	APlaceable* Build = nullptr;

	UPROPERTY()
	AAttachPoint* AttachPoint = nullptr;
};
