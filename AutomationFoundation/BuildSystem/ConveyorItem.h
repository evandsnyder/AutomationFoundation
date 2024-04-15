#pragma once
#include "AutomationFoundation/Interaction/IInteractable.h"
#include "Components/TimelineComponent.h"

#include "ConveyorItem.generated.h"

class AConveyorBelt;
class UItemSpecification;
class USplineComponent;
class UTimelineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimelineStopped);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimelineStarted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnNext);

UCLASS()
class AConveyorItem : public AActor, public IInteractable
{
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> DisplayMesh;
	

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UItemSpecification> ItemSpecification;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AConveyorBelt> ConveyorRef;

	UPROPERTY()
	float ItemInterval;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* FloatCurve;

private:
	UPROPERTY()
	TWeakObjectPtr<AConveyorItem> ItemInFront;

	UPROPERTY()
	bool bHasReachedEnd;
	UPROPERTY()
	float TrackProgress;

	UPROPERTY()
	FTimeline MovementTimeline;

	UPROPERTY()
	FTimerHandle SpawnNextTimer;

public:
	AConveyorItem();
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	FOnTimelineStarted OnTimelineStarted;
	UPROPERTY()
	FOnTimelineStopped OnTimelineStopped;
	UPROPERTY()
	FOnSpawnNext OnSpawnNext;

	UFUNCTION()
	void SpawnNext();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void SetItemSpecification(UItemSpecification* NewItemSpecification);

	/// Interactable Interface
	virtual FText GetInteractionText() override;
	virtual void OnInteract(AActor* InteractInstigator) override;

	UFUNCTION()
	void TimelineTick(float InterpolationValue);

	UFUNCTION()
	void BindEvents(AConveyorItem* NewItemInFront);

	float GetDistanceAlongSpline();
	float GetItemInterval() const;

	UFUNCTION()
	void SetItemInFront(AConveyorItem* NewItem);

	void SetupTimeline(AConveyorBelt* NewConveyorBelt);

	AConveyorItem* GetItemInFront() const;

	UFUNCTION()
	void RestartMovement();

	UFUNCTION()
	void StopMovement();
	UFUNCTION()
	void ForceStop();
	UFUNCTION()
	void OnReachConveyorEnd();

	UFUNCTION(BlueprintCallable)
	UItemSpecification* GetItemSpecification() const;

	UFUNCTION(BlueprintCallable)
	void SetItemInterval(float NewInterval);

	UFUNCTION()
	bool HasReachedEnd() const;

	UFUNCTION()
	void SetConveyor(AConveyorBelt* NewBelt);

protected:
	bool ShouldStop();

	bool IsStopped() const;
	bool IsLastItem() const;
};
