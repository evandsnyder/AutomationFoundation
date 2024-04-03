#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class AProjectileBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionBox;

	UPROPERTY()
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ProjectileFX;

	UPROPERTY(EditDefaultsOnly)
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 4000.0f;

public:
	AProjectileBase();
	virtual void BeginPlay() override;

	// Solid objects..
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                         const FHitResult& SweepResult);
};
