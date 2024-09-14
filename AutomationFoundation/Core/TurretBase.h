#pragma once

#include "CoreMinimal.h"
#include "AutomationFoundation/BuildSystem/Buildables/Placeable.h"
#include "TurretBase.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDetected, AActor*, Actor);

class USphereComponent;
class UHealthComponent;
class UInventoryComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTurret, All, Log);

UCLASS()
class AUTOMATIONFOUNDATION_API ATurretBase : public APlaceable
{
	GENERATED_BODY()

public:
	ATurretBase();

	UFUNCTION(BlueprintNativeEvent)
	void OnEnemyDetectedBegin(AActor* EnemyActor);

	UFUNCTION(BlueprintNativeEvent)
	void OnEnemyDetectedEnd(AActor* EnemyActor);

	UFUNCTION(BlueprintNativeEvent)
	void OnHealthChanged(AActor* InInstigator,
	                     UHealthComponent* OwningComponent,
	                     float UpdatedHealth, float Delta);
	void OnHealthChanged_Implementation(AActor* InInstigator,
	                                    UHealthComponent* OwningComponent,
	                                    float UpdatedHealth, float Delta);

	UFUNCTION()
	void OnDetectActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnDetectActorEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent)
	void OnEnemyKilled(AActor* EnemyUnit);

	virtual bool PlacePreview_Implementation() override;

private:
	void InitiateDeathSequence();

protected:
	UPROPERTY(EditDefaultsOnly)
	float DetectionRange = 200.0f;

	UPROPERTY(Category=Mesh, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Turret)
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Turret)
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Turret)
	USphereComponent* DetectionSphere;
};
