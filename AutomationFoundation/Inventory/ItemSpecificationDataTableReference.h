#pragma once

#include "Engine/DataTable.h"
#include "ItemSpecification.h"
#include "ItemSpecificationDataTableReference.generated.h"

USTRUCT(BlueprintType)
struct FItemSpecificationDataTableReference : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemID = "UNKNOWN";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UItemSpecification> ItemSpecificationRef;
};
