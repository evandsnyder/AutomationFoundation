#pragma once

#include "BuildShelfComponent.generated.h"

class UBuildingSpecification;
struct FBuildItemDataTableReference;

UCLASS()
class UBuildShelfComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UBuildingSpecification>> BuildShelf;

public:
	UBuildShelfComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	UBuildingSpecification* GetBuildModelByIndex(int32 Index);
};
