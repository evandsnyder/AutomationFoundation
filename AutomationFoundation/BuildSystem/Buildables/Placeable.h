#pragma once

#include "Placeable.generated.h"

class UBuildingSpecification;

UCLASS()
class APlaceable : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	UMaterial* HologramParentMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* HologramMaterial;

	UPROPERTY(BlueprintReadOnly)
	UBuildingSpecification* BuildingSpecification;

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> ActorsToIgnoreDuringPlacement;

public:
	APlaceable();

	virtual void BeginPlay() override;

	virtual void Configure(UBuildingSpecification* NewBuildingSpecification);

	UFUNCTION(BlueprintNativeEvent)
	void CreatePreview();

	UFUNCTION(BlueprintNativeEvent)
	void CancelPreview();
	UFUNCTION(BlueprintNativeEvent)
	bool PlacePreview();
	UFUNCTION(BlueprintNativeEvent)
	void DestroyPreview();

	UFUNCTION(BlueprintNativeEvent)
	void ScrollPreview(float Value);

	// Used for things like poles which have a scroll behavior that goes with them
	UFUNCTION(BlueprintNativeEvent)
	void CustomScrollPreview(float Value);

	UFUNCTION(BlueprintNativeEvent)
	bool IsPlaceable();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateLocation(const FHitResult& HitResult);

	UFUNCTION(BlueprintNativeEvent)
	void SetAllMaterialOverlays(UMaterialInterface* MaterialOverlay, bool bRemoveOverlay = false);

	UFUNCTION(BlueprintNativeEvent)
	void OnPreviewPlaced();
};
