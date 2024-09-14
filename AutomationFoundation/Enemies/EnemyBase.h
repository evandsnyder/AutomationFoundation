// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBaseDestroyed);

class UWidgetComponent;
class UAFUWHealthBar;
class UHealthComponent;
class AEnemyCharacterBase;

UCLASS()
class AUTOMATIONFOUNDATION_API AEnemyBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyBase();

	FOnBaseDestroyed OnBaseDestroyed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void OnHealthRegenerated();
	UFUNCTION()
	void OnHealthChanged(AActor* InInstigator, UHealthComponent* OwningComponent, float
	                     UpdatedHealth, float Delta);

	UFUNCTION()
	void SpawnNewEnemy();

	UFUNCTION()
	void OnEnemyUnitDeath(AEnemyCharacterBase* EnemyUnit);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyCharacterBase> EnemyToSpawn;

	UPROPERTY()
	FTimerHandle RespawnTimer;

	UPROPERTY(EditAnywhere)
	float RespawnDelay = 60.0f;

	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly)
	float HealthRegenDelay = 15.0f; // seconds

	UPROPERTY(EditDefaultsOnly)
	float HealthRegenRate = 2.0f; // seconds

	UPROPERTY(EditAnywhere)
	float HealthRegenAmount = 50.0f;

	UPROPERTY()
	FTimerHandle HealthRegenTimer;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFUWHealthBar> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxEnemiesToSpawn = 15;

	UPROPERTY()
	int32 CurrentEnemiesSpawned = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SpawnLocation;

	// TODO: This may end up being a skeletal mesh rather than a static mesh..
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BaseMesh;
};
