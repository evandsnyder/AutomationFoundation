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

	ActorsToIgnoreDuringPlacement = {};
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

void APlaceable::CreatePreview_Implementation()
{
	SetAllMaterialOverlays(HologramMaterial);
}

void APlaceable::CancelPreview_Implementation()
{
	Destroy();
}

bool APlaceable::PlacePreview_Implementation()
{
	SetAllMaterialOverlays(nullptr, true);
	OnPreviewPlaced();
	return true;
}

void APlaceable::DestroyPreview_Implementation()
{
	Destroy();
}

void APlaceable::ScrollPreview_Implementation(float Value)
{
	const FRotator NewRotation = GetActorRotation() + FRotator(0.0f, 45.0f * Value, 0.0f);
	SetActorRotation(NewRotation);
}

void APlaceable::CustomScrollPreview_Implementation(float Value)
{
}

bool APlaceable::IsPlaceable_Implementation()
{
	return true; // TODO: Remove this...
	/*
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(false, Origin, BoxExtent, true);
	::DrawDebugBox(GetWorld(), Origin, BoxExtent, FColor::Red);

	FHitResult Result;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActors(ActorsToIgnoreDuringPlacement);

	LOG_INFO(LogTemp, "Ignoring %d actors", Params.GetIgnoredActors().Num());

	bool HitSomething = GetWorld()->SweepSingleByChannel(Result, Origin, Origin, {},
	                                                     ECC_AF_General, FCollisionShape::MakeBox(BoxExtent * 0.8f), Params);

	if (HitSomething)
	{
		LOG_WARNING(LogTemp, "Hitting: %s", *Result.GetActor()->GetName());
	}

	return !HitSomething;
	*/
}

void APlaceable::UpdateLocation_Implementation(const FHitResult& HitResult)
{
	SetActorLocation(HitResult.Location);
	HologramMaterial->SetVectorParameterValue(TEXT("Color"), IsPlaceable() ? FColor::Green : FColor::Red);
}

void APlaceable::SetAllMaterialOverlays_Implementation(UMaterialInterface* MaterialOverlay, bool bRemoveOverlay)
{
	for (UActorComponent* ChildComponent : GetComponents())
	{
		if (UMeshComponent* Mesh = Cast<UMeshComponent>(ChildComponent))
		{
			Mesh->SetOverlayMaterial(bRemoveOverlay ? nullptr : MaterialOverlay);
		}
	}
}

void APlaceable::OnPreviewPlaced_Implementation()
{
	SetAllMaterialOverlays(nullptr, true);
}
