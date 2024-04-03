#pragma once
#include "AFPlaceablePreview.h"

#include "AFMountedPlaceable.generated.h"

UCLASS()
class AAFMountedPlaceable : public AAFPlaceablePreview
{
	GENERATED_BODY()

	bool bIsSnapped = false;

	UPROPERTY()
	AActor* MountTarget;

public:
	virtual bool IsValidPlacement() const override;

	virtual void OnItemPlaced(AActor* NewActor) const override;
	virtual void UpdatePosition(const FHitResult& HitResult, float GridSnapSize = 50.0f) override;

	virtual void OnActorBeginOverlap_Impl(AActor* OverlappedActor, AActor* OtherActor) override;
	virtual void OnActorEndOverlap_Impl(AActor* OverlappedActor, AActor* OtherActor) override;

private:
	void UpdateMeshColor() const;
};
