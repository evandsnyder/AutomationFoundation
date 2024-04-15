#include "SupportBeam.h"

#include "Components/InstancedStaticMeshComponent.h"

ASupportBeam::ASupportBeam()
{
	SupportBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Support Base"));
	RootComponent = SupportBase;
	SupportBase->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	SupportTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Support Top"));
	SupportTop->SetupAttachment(RootComponent);
	SupportTop->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	if (IsValid(MinimalMesh))
	{
		SupportTop->SetStaticMesh(MinimalMesh);
	}

	SupportBeam = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Support Beam"));
	SupportBeam->SetupAttachment(RootComponent);
	SupportBeam->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	AttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Attach Point"));
	AttachPoint->SetupAttachment(SupportTop);
}

void ASupportBeam::BeginPlay()
{
	Super::BeginPlay();

	SetAllMaterialOverlays(HologramMaterial);
}

bool ASupportBeam::PlacePreview()
{
	// if (IsValid(BuildingSpecification))
	// {
	// 	const FVector Location = GetActorLocation();
	// 	const FRotator Rotation = GetActorRotation();
	// 	AActor* NewActor = GetWorld()->SpawnActor(BuildingSpecification->ActorClass, &Location, &Rotation);
	// 	// Is there anything else that we need to do??
	// 	// Such as configuring this...
	// 	if (AConveyorSupportBeam* Beam = Cast<AConveyorSupportBeam>(NewActor))
	// 	{
	// 		// Make this super customizable somehow..
	// 		Beam->GenerateMeshWithNewHeight(HeightOverride);
	// 	}
	// 	return true;
	// }


	// SetActorEnableCollision(ECollisionEnabled::QueryAndPhysics);
	if (IsValid(SupportBase))
	{
		SupportBase->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	if (IsValid(SupportTop))
	{
		SupportTop->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	if (IsValid(SupportBeam))
	{
		SupportBeam->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	SetActorEnableCollision(true);
	SetAllMaterialOverlays(nullptr, false);
	return true;
}

void ASupportBeam::CreatePreview()
{
	GenerateMeshByHeight();
	SetActorEnableCollision(ECollisionEnabled::NoCollision);
}

void ASupportBeam::ScrollPreview(float Value)
{
	const FRotator NewRotation = GetActorRotation() + FRotator(0.0f, 45.0f * Value, 0.0f);
	SetActorRotation(NewRotation);
}

void ASupportBeam::CustomScrollPreview(float Value)
{
	HeightOverride += Value * 50.0f;
	HeightOverride = FMath::Clamp(HeightOverride, 0.0f, 200.0f);
	GenerateMeshByHeight();
}

void ASupportBeam::UpdateLocation(const FHitResult& HitResult)
{
	SetActorLocation(HitResult.Location.GridSnap(50.0f));
}

USceneComponent* ASupportBeam::GetAttachPoint() const
{
	return AttachPoint;
}

void ASupportBeam::GenerateMeshByHeight() const
{
	SupportBeam->ClearInstances();

	if (HeightOverride == 0.0f)
	{
		SupportTop->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		SupportBase->SetHiddenInGame(true);
		SupportBeam->SetHiddenInGame(true);

		check(IsValid(MinimalMesh));
		SupportTop->SetStaticMesh(MinimalMesh);
		return;
	}

	SupportBase->SetHiddenInGame(false);
	SupportBeam->SetHiddenInGame(false);

	const int32 LastIndex = FMath::TruncToInt(HeightOverride / 50.0f) - 1;

	for (int i = 1; i <= LastIndex; i++)
	{
		FTransform InstanceTransform;
		InstanceTransform.SetLocation(FVector(0.0f, 0.0f, i * 50.0f));

		SupportBeam->AddInstance(InstanceTransform);
	}

	SupportTop->SetStaticMesh(SupportTopMesh);

	SupportTop->SetRelativeLocation(FVector(0.0f, 0.0f, HeightOverride * 1.0f));
}
