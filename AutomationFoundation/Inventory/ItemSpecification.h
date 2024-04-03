#pragma once

#include "CoreMinimal.h"
#include "ItemClassification.h"
#include "Engine/DataAsset.h"
#include "ItemSpecification.generated.h"

UCLASS(BlueprintType)
class AUTOMATIONFOUNDATION_API UItemSpecification : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackSize;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UStaticMesh> DisplayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta=(Bitmask, BitmaskEnum =  "/Script/AutomationFoundation.EItemClassification"))
	int32 ItemClassificationFlags = 0;
};
