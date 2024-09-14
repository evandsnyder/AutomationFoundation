#include "ResourceNode.h"

AResourceNode::AResourceNode()
{
	VeinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OreVein"));
	SetRootComponent(VeinMesh);
}

URecipeSpecification* AResourceNode::GetHarvestedResource() const
{
	return Recipe;
}
