// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "EnemyCharacterBase.h"
#include "AutomationFoundation/Components/HealthComponent.h"
#include "AutomationFoundation/UserInterface/AFUWHealthBar.h"
#include "Components/WidgetComponent.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	SetRootComponent(BaseMesh);
	SpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Location"));
	SpawnLocation->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemyBase::OnHealthChanged);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Widget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Immediately spawn 2 enemies
	SpawnNewEnemy();
	SpawnNewEnemy();

	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AEnemyBase::SpawnNewEnemy, RespawnDelay, true);

	if (IsValid(HealthBarWidgetComponent))
	{
		HealthBarWidgetComponent->SetWidgetClass(HealthBarWidgetClass);

		UAFUWHealthBar* HealthBarWidget = Cast<UAFUWHealthBar>(HealthBarWidgetComponent->GetWidget());
		if (IsValid(HealthBarWidget))
		{
			HealthComponent->OnHealthChanged.AddDynamic(HealthBarWidget, &UAFUWHealthBar::OnHealthChanged);
		}
	}
}

/**
 * This function occurs whenever we are regenerating health and will continue to be invoked until we have full health
 */
void AEnemyBase::OnHealthRegenerated()
{
	HealthComponent->ApplyHealthChange(HealthRegenAmount);
	if (!HealthComponent->IsFull())
	{
		GetWorldTimerManager().SetTimer(HealthRegenTimer, this, &AEnemyBase::OnHealthRegenerated, HealthRegenRate);
	}
}

/**
 * When we receive damage or healing, this function will be invoked
 * @param InInstigator Actor instigating the health change
 * @param OwningComponent The owner of the health component (us)
 * @param UpdatedHealth What the health is now
 * @param Delta What the change in health is
 */
void AEnemyBase::OnHealthChanged(AActor* InInstigator, UHealthComponent* OwningComponent, float UpdatedHealth, float Delta)
{
	if (!HealthComponent->IsAlive())
	{
		OnBaseDestroyed.Broadcast();
		Destroy();
	}


	// Is this going up or down???
	if (Delta < 0.0f)
	{
		FTimerManager& TimerManager = GetWorldTimerManager();
		// we lost health... we need to restart the heal timer...
		if (TimerManager.IsTimerActive(HealthRegenTimer))
		{
			TimerManager.ClearTimer(HealthRegenTimer);
			TimerManager.SetTimer(HealthRegenTimer, this, &AEnemyBase::OnHealthRegenerated, HealthRegenDelay);
		}
	}
}

void AEnemyBase::SpawnNewEnemy()
{
	// Can't spawn any more enemies
	if (CurrentEnemiesSpawned >= MaxEnemiesToSpawn) { return; }

	// Get random location in range around base

	FRotator SpawnRotation; // Doesn't really matter...

	AEnemyCharacterBase* NewEnemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(EnemyToSpawn, SpawnLocation->GetComponentLocation(), SpawnRotation);

	NewEnemy->OnDeath.AddDynamic(this, &AEnemyBase::OnEnemyUnitDeath);
	OnBaseDestroyed.AddDynamic(NewEnemy, &AEnemyCharacterBase::OnHomeBaseDestroyed);
}

void AEnemyBase::OnEnemyUnitDeath(AEnemyCharacterBase* EnemyUnit)
{
	OnBaseDestroyed.RemoveDynamic(EnemyUnit, &AEnemyCharacterBase::OnHomeBaseDestroyed);
	EnemyUnit->OnDeath.RemoveDynamic(this, &AEnemyBase::OnEnemyUnitDeath);
	CurrentEnemiesSpawned--;
}
