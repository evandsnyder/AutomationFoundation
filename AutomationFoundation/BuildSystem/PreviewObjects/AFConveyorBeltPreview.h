#pragma once

#include "AFPreview.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "AFConveyorBeltPreview.generated.h"

class AAFBeltSupportPreview;
class USplineMeshComponent;

UCLASS()
class AAFConveyorBeltPreview : public AAFPreview
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	USplineMeshComponent* SplineMesh;

	UPROPERTY(BlueprintReadOnly)
	AAFBeltSupportPreview* StartSupport;

	UPROPERTY(BlueprintReadOnly)
	AAFBeltSupportPreview* EndSupport;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAFBeltSupportPreview> BeltSupportClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USplineComponent* Spline;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TempConveyor;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ConveyorStart;
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ConveyorMid;
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ConveyorEnd;

	UPROPERTY()
	bool bIsStartLockedIn = false;

	UPROPERTY()
	TMap<USplineMeshComponent*, FVector2D> SplineMeshComponents;
	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshComponentsArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SectionLengthOverride = 100.0f;

public:
	AAFConveyorBeltPreview();

	virtual void BeginPlay() override;

	virtual void CreatePreview() override;
	virtual bool PlacePreview() override;
	virtual bool IsPlaceable() override;
	virtual void CancelPreview() override;

	virtual void UpdateLocation(const FHitResult& HitResult) override;

	virtual void ScrollPreview(float Value) override;
	virtual void CustomScrollPreview(float Value) override;

	virtual void SetAllMaterialOverlays(UMaterialInterface* MaterialOverlay, bool bRemoveOverlay = false) override;

private:
	bool PlaceStartSupport();
	bool PlaceEndSupport();
	void PlaceSupport(AAFBeltSupportPreview* Support);

	void UpdateSplineShape() const;
	void UpdateSplineVisual();

	UFUNCTION(BlueprintPure)
	int32 GetLastSplinePoint() const;

	UFUNCTION(BlueprintPure)
	void GetSplineInformation(int32 PointIndex, ESplineCoordinateSpace::Type CoordinateSpace, FTransform& OutTransform, FVector& OutTangent, FVector& OutForwardVector) const;

	UFUNCTION(BlueprintPure)
	float GetDistanceBetweenTwoSplinePoints(int32 IndexA, int32 IndexB, bool bUseAbsolute = false) const;

	UFUNCTION(BlueprintPure)
	float GetRelativeRotation() const;

	UFUNCTION(BlueprintPure)
	void GetRelativeLocation(float& OutX, float& OutY) const;

	UFUNCTION(BlueprintPure)
	void GetEndQuad(float& OutX, float& OutY) const;

	UFUNCTION(BlueprintPure)
	float GetLinearDistanceOfSpline() const;

	void HandleShortUTurnBelt() const;
	void HandleMidUTurnBelt() const;
	void HandleLongUTurnBelt() const;
};
