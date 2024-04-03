#pragma once

#include "AFPlaceablePreview.generated.h"

class UBuildingSpecification;
class UItemSpecification;

UCLASS()
class AAFPlaceablePreview : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PreviewMesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* PreviewMaterial;
	UMaterialInstanceDynamic* DynamicPreviewMaterial;

	UPROPERTY()
	UItemSpecification* ItemToSpawn;

	bool bIsOverriding = false;

public:
	AAFPlaceablePreview();

	virtual void BeginPlay() override;

	UFUNCTION()
	void ChangeItem(UItemSpecification* NewItem);

	virtual void UpdatePosition(const FHitResult& HitResult, float GridSnapSize = 50.0f);

	UFUNCTION()
	bool TryPlaceItem() const;

	virtual void OnItemPlaced(AActor* NewActor) const;

	virtual void Tick(float DeltaSeconds) override;

	virtual bool IsValidPlacement() const;
	virtual void Configure(UBuildingSpecification* BuildingSpecification);

protected:
	UFUNCTION()
	void OnActorBeginOverlapActor(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void OnActorEndOverlapActor(AActor* OverlappedActor, AActor* OtherActor);

	virtual void OnActorBeginOverlap_Impl(AActor* OverlappedActor, AActor* OtherActor);
	virtual void OnActorEndOverlap_Impl(AActor* OverlappedActor, AActor* OtherActor);
};
