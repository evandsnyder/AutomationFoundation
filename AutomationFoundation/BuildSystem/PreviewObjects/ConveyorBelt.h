#pragma once

#include "Placeable.h"
#include "AutomationFoundation/BuildSystem/BuildInformation.h"
#include "AutomationFoundation/BuildSystem/EBuildDirection.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "ConveyorBelt.generated.h"

class AConveyorItem;
class AAttachPoint;
class ASupportBeam;
class USplineMeshComponent;

UCLASS()
class AConveyorBelt : public APlaceable
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(BlueprintReadOnly)
	ASupportBeam* StartSupport;

	UPROPERTY(BlueprintReadOnly)
	ASupportBeam* EndSupport;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASupportBeam> BeltSupportClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USplineComponent* Spline;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	UStaticMeshComponent* TempConveyor;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	UStaticMesh* ConveyorStart;
	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	UStaticMesh* ConveyorMid;
	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	UStaticMesh* ConveyorEnd;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	UMaterialInterface* BeltMaterialClass;
	UPROPERTY(BlueprintReadOnly)
	UMaterialInstanceDynamic* BeltMaterial;

	UPROPERTY()
	float Speed = 100.0f;

	UPROPERTY()
	bool bIsStartLockedIn = false;

	UPROPERTY()
	TMap<USplineMeshComponent*, FVector2D> SplineMeshComponents;
	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshComponentsArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SectionLengthOverride = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	TSubclassOf<AAttachPoint> AttachPointClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	AAttachPoint* MyStartAttachPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	AAttachPoint* MyEndAttachPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	AAttachPoint* OtherStartAttachPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	AAttachPoint* OtherEndAttachPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	EBuildDirection StartDirection;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	EBuildDirection EndDirection;

	UPROPERTY()
	FTimerHandle ItemRequestTimer;

public:
	AConveyorBelt();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void CreatePreview() override;
	virtual bool PlacePreview() override;
	virtual bool IsPlaceable() override;
	virtual void CancelPreview() override;

	virtual void UpdateLocation(const FHitResult& HitResult) override;

	virtual void ScrollPreview(float Value) override;
	virtual void CustomScrollPreview(float Value) override;

	virtual void SetAllMaterialOverlays(UMaterialInterface* MaterialOverlay, bool bRemoveOverlay = false) override;

	UFUNCTION()
	USplineComponent* GetSpline() const;

	float GetSpeed() const;

	FTransform GetTransformByAlpha(float Alpha) const;
	float GetLengthByAlpha(float Alpha) const;

	AConveyorItem* GetLastItem() const;
	void OnItemPickedUp(AConveyorItem* Item);
	void AddItemToConveyor(AConveyorItem* NewItem, bool bUpdateLast = false);

	bool IsInputConnected() const;
	bool IsOutputConnected() const;

	bool CanSpawnNextItem();

	UFUNCTION()
	bool GetNextBuild(FBuildInformation& OutBuildInformation);
	UFUNCTION()
	bool GetPreviousBuild(FBuildInformation& OutBuildInformation);

	UFUNCTION()
	void MoveItemToNextConveyor(AConveyorItem* MovedItem);

private:
	void UpdateStartLocation(const FHitResult& HitResult);
	void UpdateEndLocation(const FHitResult& HitResult);

	bool PlaceStartSupport();
	bool PlaceEndSupport();
	void PlaceSupport(ASupportBeam* Support);

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

	FTimerHandle SpawnItemTimer;

	void CreateAttachPoints();

	EBuildDirection InvertDirection(EBuildDirection InDirection) const;

	void SetupLinkedBuilds();
	void SetupLinkedAttachPoint(AAttachPoint* AttachPoint);
	void ItemConsumed();

	void UpdateLastItemInQueue(AConveyorItem* NewItem);

protected:
	UPROPERTY()
	AConveyorItem* LastItemInQueue;
	UPROPERTY()
	TArray<AConveyorItem*> ItemsOnConveyor;
};
