#include "AFMountedPlaceable.h"

#include "MountComponent.h"

bool AAFMountedPlaceable::IsValidPlacement() const
{
	return bIsSnapped;
}

void AAFMountedPlaceable::OnItemPlaced(AActor* NewActor) const
{
	// if (IPlaceable* Placeable = Cast<IPlaceable>(NewActor))
	// {
	// 	Placeable->OnPlaced(MountTarget);
	// }
}

void AAFMountedPlaceable::UpdatePosition(const FHitResult& HitResult, float GridSnapSize)
{
	if (AActor* Actor = HitResult.GetActor())
	{
		if (const UMountComponent* MountComponent = Cast<UMountComponent>(Actor->GetComponentByClass(UMountComponent::StaticClass())))
		{
			MountTarget = Actor;
			SetActorLocation(MountComponent->GetComponentLocation());
			if (!bIsSnapped)
			{
				bIsSnapped = true;
				UpdateMeshColor();
			}
			return;
		}
	}

	if (bIsSnapped)
	{
		bIsSnapped = false;
		UpdateMeshColor();
	}
	MountTarget = nullptr;
	Super::UpdatePosition(HitResult, GridSnapSize);
}

void AAFMountedPlaceable::OnActorBeginOverlap_Impl(AActor* OverlappedActor, AActor* OtherActor)
{
	UpdateMeshColor();
}

void AAFMountedPlaceable::OnActorEndOverlap_Impl(AActor* OverlappedActor, AActor* OtherActor)
{
	bIsSnapped = false;
	UpdateMeshColor();
}

void AAFMountedPlaceable::UpdateMeshColor() const
{
	if (IsValid(DynamicPreviewMaterial))
		DynamicPreviewMaterial->SetVectorParameterValue("HighlightColor", bIsSnapped ? FColor::Green : FColor::Red);
}
