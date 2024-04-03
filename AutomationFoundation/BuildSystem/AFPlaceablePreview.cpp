#include "AFPlaceablePreview.h"

#include "BuildingSpecification.h"
#include "IPlaceable.h"
#include "AutomationFoundation/Inventory/ItemSpecification.h"

AAFPlaceablePreview::AAFPlaceablePreview()
{
	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetGenerateOverlapEvents(true);


	OnActorBeginOverlap.AddDynamic(this, &AAFPlaceablePreview::OnActorBeginOverlapActor);
	OnActorEndOverlap.AddDynamic(this, &AAFPlaceablePreview::OnActorEndOverlapActor);
	bIsOverriding = false;
}

void AAFPlaceablePreview::BeginPlay()
{
	Super::BeginPlay();
	if (PreviewMaterial)
	{
		DynamicPreviewMaterial = UMaterialInstanceDynamic::Create(PreviewMaterial, this);
		DynamicPreviewMaterial->SetVectorParameterValue(FName("HighlightColor"), FColor::Green);
		if (PreviewMesh)
		{
			PreviewMesh->SetMaterial(0, DynamicPreviewMaterial);
		}
	}
}

void AAFPlaceablePreview::ChangeItem(UItemSpecification* NewItem)
{
	ItemToSpawn = NewItem;
	if (!IsValid(ItemToSpawn)) return;
	PreviewMesh->SetStaticMesh(ItemToSpawn->DisplayMesh.LoadSynchronous());
	if (DynamicPreviewMaterial)
	{
		PreviewMesh->SetMaterial(0, DynamicPreviewMaterial);
	}
}

void AAFPlaceablePreview::UpdatePosition(const FHitResult& HitResult, float GridSnapSize)
{
	SetActorLocation(HitResult.Location.GridSnap(GridSnapSize));
}

bool AAFPlaceablePreview::TryPlaceItem() const
{
	if (!IsValidPlacement()) return false;

	const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = GetActorRotation();
	AActor* NewActor = GetWorld()->SpawnActor(ItemToSpawn->ActorClass, &SpawnLocation, &SpawnRotation);
	if (!NewActor) return false;

	OnItemPlaced(NewActor);

	return true;
}

void AAFPlaceablePreview::OnItemPlaced(AActor* NewActor) const
{
	if (IPlaceable* Placeable = Cast<IPlaceable>(NewActor))
	{
		Placeable->OnPlaced();
	}
}

void AAFPlaceablePreview::Tick(float DeltaSeconds)
{
}

bool AAFPlaceablePreview::IsValidPlacement() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	return OverlappingActors.IsEmpty();
}

void AAFPlaceablePreview::Configure(UBuildingSpecification* BuildingSpecification)
{
	// Need to copy the meshes from the Actor...
	// TODO: Maybe store the mesh..
	// Maybe we just keep a static display mesh???

	// OR, machines should have each have their own "activate" field, where we enable all interaction fields...
	// Don't particularly like either idea
}

void AAFPlaceablePreview::OnActorBeginOverlapActor(AActor* OverlappedActor, AActor* OtherActor)
{
	OnActorBeginOverlap_Impl(OverlappedActor, OtherActor);
}

void AAFPlaceablePreview::OnActorEndOverlapActor(AActor* OverlappedActor, AActor* OtherActor)
{
	OnActorEndOverlap_Impl(OverlappedActor, OtherActor);
}

void AAFPlaceablePreview::OnActorBeginOverlap_Impl(AActor* OverlappedActor, AActor* OtherActor)
{
	bIsOverriding = !IsValidPlacement();
	if (bIsOverriding && IsValid(DynamicPreviewMaterial))
	{
		DynamicPreviewMaterial->SetVectorParameterValue("HighlightColor", FColor::Red);
	}
}

void AAFPlaceablePreview::OnActorEndOverlap_Impl(AActor* OverlappedActor, AActor* OtherActor)
{
	bIsOverriding = !IsValidPlacement();
	if (!bIsOverriding && IsValid(DynamicPreviewMaterial))
	{
		DynamicPreviewMaterial->SetVectorParameterValue(FName("HighlightColor"), FColor::Green);
	}
}
