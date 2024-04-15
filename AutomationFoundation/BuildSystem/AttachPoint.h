#pragma once
#include "EBuildDirection.h"

#include "AttachPoint.generated.h"

class APlaceable;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class AAttachPoint : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	USceneComponent* SceneRoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ArrowMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	UMaterialInterface* MaterialClass;

	UPROPERTY()
	UMaterialInstanceDynamic* ArrowMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	bool bShowArrow = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	FVector BoxSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	FVector LocationOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	EBuildDirection Direction = EBuildDirection::Unspecified;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Builds")
	APlaceable* ParentBuild;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Builds")
	APlaceable* ConnectedBuild;

public:
	AAttachPoint();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateSize(const FVector& NewBoxSize, const FVector& NewLocationOffset);

	UFUNCTION()
	void OnBuildsLinked();
	
	
	bool CanConnect(EBuildDirection InDirection) const;
	bool TryLinkConnectedBuilds(AAttachPoint* OtherAttachPoint);

	void SetParentBuild(APlaceable* NewParent);
	void SetDirection(EBuildDirection NewDirection);
	void SetConnectedBuild(APlaceable* NewConnectedBuild);

	APlaceable* GetParentBuild() const;
	EBuildDirection GetDirection() const;
	APlaceable* GetConnectedBuild() const;
};
