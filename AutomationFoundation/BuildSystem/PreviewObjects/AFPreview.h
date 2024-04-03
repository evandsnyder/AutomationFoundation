#pragma once

#include "AFPreview.generated.h"

class UBuildingSpecification;

UCLASS()
class AAFPreview : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	UMaterial* HologramParentMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* HologramMaterial;

	UPROPERTY(BlueprintReadOnly)
	UBuildingSpecification* BuildingSpecification;

public:
	AAFPreview();

	virtual void BeginPlay() override;

	virtual void Configure(UBuildingSpecification* NewBuildingSpecification);

	virtual void CreatePreview();
	virtual void CancelPreview();
	virtual bool PlacePreview();
	virtual void DestroyPreview();

	virtual void ScrollPreview(float Value);

	// Used for things like poles which have a scroll behavior that goes with them
	virtual void CustomScrollPreview(float Value);

	virtual bool IsPlaceable();

	virtual void UpdateLocation(const FHitResult& HitResult);

	virtual void SetAllMaterialOverlays(UMaterialInterface* MaterialOverlay, bool bRemoveOverlay = false);
};
