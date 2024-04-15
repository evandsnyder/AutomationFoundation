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

	UPROPERTY()
	int32 CurrentIndex = 0;

public:
	UBuildShelfComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	UBuildingSpecification* UpdateIndexAndGetBuildModel(int32 Index);

	int32 GetCurrentIndex() const;
};
