#include "Placeable.h"

#include "AutomationFoundation/AutomationFoundation.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"

APlaceable::APlaceable()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(
		TEXT("/Script/Engine.Material'/Game/AutomationFoundation/BuildSystem/Materials/M_Hologram.M_Hologram'"));
	if (MaterialFinder.Succeeded())
	{
		HologramParentMaterial = MaterialFinder.Object;
	}
	else
	{
		LOG_WARNING(LogTemp, "Could not find base hologram material");
	}
}

void APlaceable::BeginPlay()
{
	Super::BeginPlay();


	if (IsValid(HologramParentMaterial))
	{
		HologramMaterial = UMaterialInstanceDynamic::Create(HologramParentMaterial, this);
	}
}

void APlaceable::Configure(UBuildingSpecification* NewBuildingSpecification)
{
	BuildingSpecification = NewBuildingSpecification;
}

void APlaceable::CreatePreview()
{
}

void APlaceable::CancelPreview()
{
	Destroy();
}

bool APlaceable::PlacePreview()
{
	return true;
}

void APlaceable::DestroyPreview()
{
	Destroy();
}

void APlaceable::ScrollPreview(float Value)
{
}

void APlaceable::CustomScrollPreview(float Value)
{
}

bool APlaceable::IsPlaceable()
{
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(false, Origin, BoxExtent, true);

	FHitResult Result;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);


	return !GetWorld()->SweepSingleByChannel(Result, Origin, Origin, {},
		ECC_AF_General, FCollisionShape::MakeBox(BoxExtent * 0.9f), Params);
}

void APlaceable::UpdateLocation(const FHitResult& HitResult)
{
}

void APlaceable::SetAllMaterialOverlays(UMaterialInterface* MaterialOverlay, bool bRemoveOverlay)
{
	for (UActorComponent* ChildComponent : GetComponents())
	{
		if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(ChildComponent))
		{
			Mesh->SetOverlayMaterial(bRemoveOverlay ? nullptr : MaterialOverlay);
		}
	}
}
