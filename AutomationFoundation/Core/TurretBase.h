#pragma once

#include "CoreMinimal.h"
#include "TurretBase.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDetected, AActor*, Actor);

class USphereComponent;
class UHealthComponent;
class UInventoryComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTurret, All, Log);

UCLASS()
class AUTOMATIONFOUNDATION_API ATurretBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category=Mesh, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Turret, meta=(AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Turret, meta=(AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Turret, meta=(AllowPrivateAccess = "true"))
	USphereComponent* DetectionSphere;

protected:
	UPROPERTY(EditDefaultsOnly)
	float DetectionRange = 200.0f;

public:
	ATurretBase();
	// virtual void BeginPlay() override;

	// UPROPERTY()
	// FOnEnemyDetected OnEnemyDetected;

	UFUNCTION(BlueprintNativeEvent)
	void OnEnemyDetectedBegin(AActor* EnemyActor);

	UFUNCTION(BlueprintNativeEvent)
	void OnEnemyDetectedEnd();

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

private:
	void InitiateDeathSequence();
};
