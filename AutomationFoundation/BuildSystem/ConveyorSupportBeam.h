#pragma once

#include "ConveyorSupportBeam.generated.h"

class UStaticMeshComponent;
class UInstancedStaticMeshComponent;

UCLASS()
class AConveyorSupportBeam : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	float Height = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* SupportBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* SupportTop;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInstancedStaticMeshComponent* SupportBeam;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* AttachPoint;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MinimalMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* SupportTopMesh;

public:
	AConveyorSupportBeam();

	void GenerateMeshWithNewHeight(float NewHeight);
};
