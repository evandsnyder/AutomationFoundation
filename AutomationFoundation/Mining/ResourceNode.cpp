#include "ResourceNode.h"

#include "AutomationFoundation/BuildSystem/MountComponent.h"

AResourceNode::AResourceNode()
{

	VeinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OreVein"));
	RootComponent = VeinMesh;

	ExtractorMountPoint = CreateDefaultSubobject<UMountComponent>(TEXT("MountPoint"));
	ExtractorMountPoint->SetupAttachment(RootComponent);
	ExtractorMountPoint->SetRelativeLocation(FVector(0.0f, 0.0f, 5.f));
}

UItemSpecification* AResourceNode::GetHarvestedResource() const
{
	return HarvestedResource;
}
