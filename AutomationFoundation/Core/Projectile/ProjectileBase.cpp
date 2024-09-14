#include "ProjectileBase.h"

#include "AutomationFoundation/Components/HealthComponent.h"
#include "AutomationFoundation/Core/AutomationFoundationTags.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Core/TurretBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AProjectileBase::AProjectileBase()
{
	Tags.Add(AFTags::Projectile);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRoot"));
	RootComponent = SceneComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(ProjectileMesh);

	ProjectileFX = CreateDefaultSubobject<UParticleSystem>(TEXT("Impact Effect System"));

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	MovementComponent->InitialSpeed = ProjectileSpeed;
	MovementComponent->MaxSpeed = ProjectileSpeed;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// CollisionBox->SetCollisionObjectType()
	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectileBase::OnProjectileHit);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnProjectileOverlap);
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsValid(ProjectileFX))
	{
		LOG_INFO(LogTemp, "Projectile Collided");
		const FTransform SpawnTransform(FRotator::ZeroRotator, GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileFX, SpawnTransform);
	}

	Destroy();
}

void AProjectileBase::OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor->ActorHasTag(AFTags::Projectile) || GetInstigator() == OtherActor || IsOwnedBy(OtherActor)) return;

	if (UHealthComponent* HealthComponent = OtherActor->GetComponentByClass<UHealthComponent>())
	{
		HealthComponent->ApplyHealthChange(BaseDamage);
		if (!HealthComponent->IsAlive())
		{
			if (ATurretBase* Turret = Cast<ATurretBase>(GetOwner()))
			{
				Turret->OnEnemyKilled(OtherActor);
			}
		}
	}

	Destroy();
}
