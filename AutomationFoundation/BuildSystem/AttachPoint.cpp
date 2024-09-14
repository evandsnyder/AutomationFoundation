#include "AttachPoint.h"

#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "Components/BoxComponent.h"
#include "Buildables/Placeable.h"

AAttachPoint::AAttachPoint()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	ArrowMesh->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetHiddenInGame(false);

	BoxSize = FVector(15.0, 62.f, 35.0f);

	LocationOffset = FVector(-15.f, 0.0f, -20.f);

	UpdateSize(BoxSize, LocationOffset);
}

void AAttachPoint::OnConstruction(const FTransform& Transform)
{
	UpdateSize(BoxSize, LocationOffset);
}

void AAttachPoint::BeginPlay()
{
	Super::BeginPlay();
}

void AAttachPoint::UpdateSize(const FVector& NewBoxSize, const FVector& NewLocationOffset)
{
	BoxSize = NewBoxSize;
	LocationOffset = NewLocationOffset;

	BoxComponent->SetRelativeLocation(FVector(BoxSize.X + LocationOffset.X, LocationOffset.Y, LocationOffset.Z));
	BoxComponent->SetBoxExtent(BoxSize);

	UpdateArrowVisibility();
}

void AAttachPoint::OnBuildsLinked()
{
	LOG_INFO(LogTemp, "%s was connected", *GetName())
}

bool AAttachPoint::CanConnect(EBuildDirection InDirection) const
{
	return !IsValid(ConnectedBuild) && (InDirection != Direction || InDirection == EBuildDirection::Unspecified);
}

bool AAttachPoint::TryLinkConnectedBuilds(AAttachPoint* NewLinkedAttachPoint)
{
	if (!IsValid(NewLinkedAttachPoint))
	{
		LOG_WARNING(LogTemp, "Could not link attach points because new linked attach point is invalid");
		return false;
	}
	if (!IsValid(ParentBuild))
	{
		LOG_WARNING(LogTemp, "Could not link attach points because parent build is invalid")
		return false;
	}

	ConnectedBuild = NewLinkedAttachPoint->ParentBuild;
	NewLinkedAttachPoint->ConnectedBuild = ParentBuild;

	return true;
}

void AAttachPoint::SetParentBuild(APlaceable* NewParent)
{
	ParentBuild = NewParent;
}

void AAttachPoint::SetDirection(EBuildDirection NewDirection)
{
	Direction = NewDirection;
	UpdateArrowVisibility();
}

void AAttachPoint::SetConnectedBuild(APlaceable* NewConnectedBuild)
{
	ConnectedBuild = NewConnectedBuild;
}

APlaceable* AAttachPoint::GetParentBuild() const
{
	return ParentBuild;
}

EBuildDirection AAttachPoint::GetDirection() const
{
	return Direction;
}

APlaceable* AAttachPoint::GetConnectedBuild() const
{
	return ConnectedBuild;
}

void AAttachPoint::UpdateArrowVisibility()
{
	const bool bShouldShowArrow = Direction != EBuildDirection::Unspecified && bShowArrow;
	ArrowMesh->SetHiddenInGame(!bShouldShowArrow);

	if (!bShouldShowArrow)
	{
		return;
	}

	if (IsValid(MaterialClass))
	{
		ArrowMaterial = UMaterialInstanceDynamic::Create(MaterialClass, this);
		FColor HighlightColor = FColor::Black;
		switch (Direction)
		{
		case EBuildDirection::Input:
			HighlightColor = FColor::Emerald;
			break;
		case EBuildDirection::Output:
			HighlightColor = FColor::Orange;
			break;
		default:
			break;
		}
		ArrowMaterial->SetVectorParameterValue("Color", HighlightColor);
		ArrowMesh->SetMaterial(0, ArrowMaterial);
	}

	if (Direction == EBuildDirection::Input)
	{
		ArrowMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
}
