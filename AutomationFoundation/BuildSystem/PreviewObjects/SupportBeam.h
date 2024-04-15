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

	virtual bool PlacePreview() override;
	virtual void CreatePreview() override;
	virtual void ScrollPreview(float Value) override;
	virtual void CustomScrollPreview(float Value) override;
	virtual void UpdateLocation(const FHitResult& HitResult) override;

	UFUNCTION()
	USceneComponent* GetAttachPoint() const; 

private:
	void GenerateMeshByHeight() const;
	
};
