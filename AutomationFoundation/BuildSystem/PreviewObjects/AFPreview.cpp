#include "AFPreview.h"

#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"

AAFPreview::AAFPreview()
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

void AAFPreview::BeginPlay()
{
	Super::BeginPlay();


	if (IsValid(HologramParentMaterial))
	{
		HologramMaterial = UMaterialInstanceDynamic::Create(HologramParentMaterial, this);
	}
}

void AAFPreview::Configure(UBuildingSpecification* NewBuildingSpecification)
{
	BuildingSpecification = NewBuildingSpecification;
}

void AAFPreview::CreatePreview()
{
}

void AAFPreview::CancelPreview()
{
	Destroy();
}

bool AAFPreview::PlacePreview()
{
	return true;
}

void AAFPreview::DestroyPreview()
{
	Destroy();
}

void AAFPreview::ScrollPreview(float Value)
{
}

void AAFPreview::CustomScrollPreview(float Value)
{
}

bool AAFPreview::IsPlaceable()
{
	return true;
}

void AAFPreview::UpdateLocation(const FHitResult& HitResult)
{
}

void AAFPreview::SetAllMaterialOverlays(UMaterialInterface* MaterialOverlay, bool bRemoveOverlay)
{
	for (UActorComponent* ChildComponent : GetComponents())
	{
		if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(ChildComponent))
		{
			Mesh->SetOverlayMaterial(bRemoveOverlay ? nullptr : MaterialOverlay);
		}
	}
}
