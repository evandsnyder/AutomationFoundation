#pragma once
#include "Placeable.h"

#include "SupportBeam.generated.h"

class USceneComponent;
class UInstancedStaticMeshComponent;
class UStaticMeshComponent;

UCLASS()
class ASupportBeam : public APlaceable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SupportBase;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SupportTop;

	UPROPERTY(EditDefaultsOnly)
	UInstancedStaticMeshComponent* SupportBeam;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* AttachPoint;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MinimalMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* SupportTopMesh;

	UPROPERTY(EditDefaultsOnly)
	float HeightOverride = 0.0f;

public:
	ASupportBeam();

	virtual void BeginPlay() override;

	virtual bool PlacePreview_Implementation() override;
	virtual void CreatePreview_Implementation() override;
	virtual void ScrollPreview_Implementation(float Value) override;
	virtual void CustomScrollPreview_Implementation(float Value) override;
	virtual void UpdateLocation_Implementation(const FHitResult& HitResult) override;

	UFUNCTION()
	USceneComponent* GetAttachPoint() const; 

private:
	void GenerateMeshByHeight() const;
	
};
