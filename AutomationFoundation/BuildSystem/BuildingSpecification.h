#pragma once

#include "BuildingSpecification.generated.h"

class AAFPreview;

UCLASS(BlueprintType)
class UBuildingSpecification : public UDataAsset
{
	GENERATED_BODY()

public:
	// Unique ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FName BuildingID;

	// This is the preview class to use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSubclassOf<AAFPreview> PreviewClass;

	// This is the actor that will spawn...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSoftObjectPtr<UTexture2D> BuildingIcon;

public:
};
