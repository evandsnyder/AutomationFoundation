#pragma once

#include "Engine/DataTable.h"
#include "BuildingSpecification.h"
#include "BuildingSpecificationDataTableReference.generated.h"

USTRUCT(BlueprintType)
struct FBuildingSpecificationDataTableReference : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BuildingID = "UNKNOWN";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UBuildingSpecification> BuildingSpecificationRef;
};
