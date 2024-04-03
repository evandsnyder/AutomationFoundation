#pragma once

#include "CoreMinimal.h"
#include "PickupBase.generated.h"

UCLASS()
class APickupBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
	class USphereComponent* CollectionSphere;

	TObjectPtr<class AAutomationFoundationCharacter> PlayerCharacter;

public:
	APickupBase();

protected:
	UFUNCTION()
	virtual void BeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult
	);

	virtual void OnPlayerBeginOverlap();

	UFUNCTION()
	virtual void EndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex
	);

	virtual void OnPlayerEndOverlap();
};
