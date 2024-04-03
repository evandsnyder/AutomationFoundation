#include "TurretBase.h"

#include "AutomationFoundationUtilities.h"
#include "AutomationFoundation/Components/HealthComponent.h"
#include "AutomationFoundation/Enemies/EnemyCharacterBase.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY(LogTurret)

ATurretBase::ATurretBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Turret Mesh"));
	// Mesh->SetupAttachment(RootComponent);
	RootComponent = Mesh;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Sphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(DetectionRange);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATurretBase::OnDetectActor);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ATurretBase::OnDetectActorEnd);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATurretBase::OnHealthChanged);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	InventoryComponent->SetInventorySize(2);
	// TODO: Set Inventory Filters??
}

void ATurretBase::OnHealthChanged_Implementation(AActor* InInstigator, UHealthComponent* OwningComponent,
                                                 float UpdatedHealth, float Delta)
{
	if (!HealthComponent->IsAlive())
	{
		InitiateDeathSequence();
	}
}

void ATurretBase::OnDetectActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		LOG_INFO(LogTurret, "Enemy Detected");
		OnEnemyDetectedBegin(OtherActor);
	}
}

void ATurretBase::OnDetectActorEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		LOG_INFO(LogTurret, "Enemy Lost");
		OnEnemyDetectedEnd();
	}
}

void ATurretBase::OnEnemyDetectedBegin_Implementation(AActor* EnemyActor)
{
}

void ATurretBase::OnEnemyDetectedEnd_Implementation()
{
}


void ATurretBase::InitiateDeathSequence()
{
	// Play death anim montage
}
