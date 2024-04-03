#pragma once
#include "TransporterBase.h"

#include "RoboticTransporter.generated.h"

UCLASS()
class ARoboticTransporter : public ATransporterBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBoxComponent> SourceDetectionBox;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBoxComponent> DestinationDetectionBox;

public:
	ARoboticTransporter();

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSourceDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                                   bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSourceDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
	void OnDestinationDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                                        bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnDestinationDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                      UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};
