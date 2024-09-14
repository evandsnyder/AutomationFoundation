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

	/**
	 * Called to instantiate the preview. Conducts setup specific to the conveyor Belt
	 */
	virtual void CreatePreview_Implementation() override;

	/**
	 * Attempts to finalize the location of this item in the game world
	 * @return true if the preview was placed successfully, false otherwise 
	 */
	virtual bool PlacePreview_Implementation() override;

	/**
	 * Checks whether we can place the conveyor belt at it's current position
	 * @return true if the conveyor belt is placeable
	 */
	virtual bool IsPlaceable_Implementation() override;

	/**
	 * Cleans up this preview object and can only be called before the object is placed
	 */
	virtual void CancelPreview_Implementation() override;

	/**
	 * Updates the location of the conveyor belt. If the staring post has already been placed, this will update the spline shape and preview as well
	 * @param HitResult Information on the new location
	 */
	virtual void UpdateLocation_Implementation(const FHitResult& HitResult) override;

	/**
	 * Rotates the conveyor belt support beam
	 * @param Value 
	 */
	virtual void ScrollPreview_Implementation(float Value) override;

	/**
	 * Changes the conveyor belt support beam elevation
	 * @param Value 
	 */
	virtual void CustomScrollPreview_Implementation(float Value) override;


	/**
	 * Updates the overlay for all materials on the actor
	 * @param MaterialOverlay Material to use
	 * @param bRemoveOverlay removes all overlays if true
	 */
	virtual void SetAllMaterialOverlays_Implementation(UMaterialInterface* MaterialOverlay, bool bRemoveOverlay = false) override;

	/**
	 * Get a pointer to the spline
	 * @return Pointer to the spline component
	 */
	UFUNCTION()
	USplineComponent* GetSpline() const;

	/**
	 * Get the spline speed
	 * @return Speed
	 */
	float GetSpeed() const;

	/**
	 * Gets the transform of a given location on the spline according to the alpha value
	 * @param Alpha range from 0 to 1 where 0 is the start of the conveyor and 1 is the end
	 * @return Transform at that spline point
	 */
	FTransform GetTransformByAlpha(float Alpha) const;

	/**
	 * Gets the length of the spline at a point along the spline
	 * @param Alpha range from 0 to 1 where 0 is the start of the conveyor and 1 is the end
	 * @return length of the spline to that point
	 */
	float GetLengthByAlpha(float Alpha) const;

	AConveyorItem* GetLastItem() const;

	/**
	 * Invoked when an item is taken off of the conveyor belt. This is used to ensure the belt continues to operate gracefully and doesn't leave any dangling pointers
	 * @param Item The removed item
	 */
	void OnItemPickedUp(AConveyorItem* Item);

	/**
	 * Called when a new item is added onto the belt
	 * @param NewItem The new item added
	 * @param bUpdateLast true if this is the last item on the chain of connected belts. Usually invoked by an output node (storage container or machine) that is playing items onto the belt
	 */
	void AddItemToConveyor(AConveyorItem* NewItem, bool bUpdateLast = false);

	bool IsInputConnected() const;
	bool IsOutputConnected() const;

	bool CanSpawnNextItem();

	/**
	 * Gets the next connected structure, if any. This may be a conveyor belt or machine of some sort
	 * @param OutBuildInformation 
	 * @return 
	 */
	UFUNCTION()
	bool GetNextBuild(FBuildInformation& OutBuildInformation);

	/**
	 * Gets the next connected structure, if any. This may be a conveyor belt or machine of some sort
	 * @param OutBuildInformation 
	 * @return 
	 */
	UFUNCTION()
	bool GetPreviousBuild(FBuildInformation& OutBuildInformation);

	/**
	 * If the next connected build is a conveyor belt, this is used to move the first item on this belt to the end of that belt 
	 * @param MovedItem 
	 */
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
