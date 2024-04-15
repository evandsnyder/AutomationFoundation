#include "AttachPoint.h"

#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "Components/BoxComponent.h"
#include "PreviewObjects/Placeable.h"

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
}

void AAttachPoint::BeginPlay()
{
	Super::BeginPlay();
}

void AAttachPoint::UpdateSize(const FVector& NewBoxSize, const FVector& NewLocationOffset)
{
	BoxSize = NewBoxSize;
	LocationOffset = NewLocationOffset;

	const bool bShouldShowArrow = Direction != EBuildDirection::Unspecified && bShowArrow;

	ArrowMesh->SetHiddenInGame(!bShouldShowArrow);

	if (!bShouldShowArrow)
	{
		return;
	}

	if (IsValid(MaterialClass))
	{
		ArrowMaterial = UMaterialInstanceDynamic::Create(MaterialClass, this);
		ArrowMaterial->SetVectorParameterValue("Color", Direction == EBuildDirection::Unspecified ? FColor::Black : FColor::Emerald);
		ArrowMesh->SetMaterial(0, ArrowMaterial);
	}

	if (Direction == EBuildDirection::Input)
	{
		ArrowMesh->SetRelativeRotation(FRotator(0.f, 0.f, 180.f));
	}

	BoxComponent->SetRelativeLocation(FVector(BoxSize.X + LocationOffset.X, LocationOffset.Y, LocationOffset.Z));
	BoxComponent->SetBoxExtent(BoxSize);
}

void AAttachPoint::OnBuildsLinked()
{
	LOG_INFO(LogTemp, "%s was connected", *GetName())
}

bool AAttachPoint::CanConnect(EBuildDirection InDirection) const
{
	return !IsValid(ConnectedBuild) && (InDirection != Direction || InDirection == EBuildDirection::Unspecified);
}

bool AAttachPoint::TryLinkConnectedBuilds(AAttachPoint* OtherAttachPoint)
{
	if (!IsValid(OtherAttachPoint) || !IsValid(ParentBuild))
		return false;

	ConnectedBuild = OtherAttachPoint->ParentBuild;
	OtherAttachPoint->ConnectedBuild = ParentBuild;

	return true;
}

void AAttachPoint::SetParentBuild(APlaceable* NewParent)
{
	ParentBuild = NewParent;
}

void AAttachPoint::SetDirection(EBuildDirection NewDirection)
{
	Direction = NewDirection;
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
