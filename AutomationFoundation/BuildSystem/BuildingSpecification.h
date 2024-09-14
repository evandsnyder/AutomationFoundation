#pragma once

#include "BuildingSpecification.generated.h"

class APlaceable;

UCLASS(BlueprintType)
class UBuildingSpecification : public UDataAsset
{
	GENERATED_BODY()

public:
	// Unique ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FName BuildingID;

	// TODO: Should combine this and the ActorClass. No need for both
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSubclassOf<APlaceable> PreviewClass;

	// This is the actor that will spawn...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSoftObjectPtr<UTexture2D> BuildingIcon;

public:
};
