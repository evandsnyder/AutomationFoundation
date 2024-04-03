#include "ConveyorSupportBeam.h"

#include "Components/InstancedStaticMeshComponent.h"

AConveyorSupportBeam::AConveyorSupportBeam()
{
	SupportBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Support Base"));
	RootComponent = SupportBase;
	SupportBase->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	SupportTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Support Top"));
	SupportTop->SetupAttachment(RootComponent);
	SupportTop->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	SupportBeam = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Support Beam"));
	SupportBeam->SetupAttachment(RootComponent);
	SupportBeam->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	AttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Attach Point"));
	AttachPoint->SetupAttachment(SupportTop);
}

void AConveyorSupportBeam::GenerateMeshWithNewHeight(float NewHeight)
{
	SupportBeam->ClearInstances();
	Height = NewHeight;

	if (Height == 0.0f)
	{
		SupportTop->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		check(IsValid(MinimalMesh));
		SupportTop->SetStaticMesh(MinimalMesh);

		SupportBase->SetHiddenInGame(true);
		SupportBeam->SetHiddenInGame(true);

		return;
	}

	SupportBase->SetHiddenInGame(false);
	SupportBeam->SetHiddenInGame(false);

	const int32 LastIndex = FMath::TruncToInt(Height / 50.0f) - 1;

	for (int i = 1; i <= LastIndex; i++)
	{
		FTransform InstanceTransform;
		InstanceTransform.SetLocation(FVector(0.0f, 0.0f, i * 50.0f));

		SupportBeam->AddInstance(InstanceTransform);
	}

	SupportTop->SetStaticMesh(SupportTopMesh);

	SupportTop->SetRelativeLocation(FVector(0.0f, 0.0f, Height * 1.0f));
}
