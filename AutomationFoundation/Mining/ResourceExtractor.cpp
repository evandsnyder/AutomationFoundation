#include "ResourceExtractor.h"

#include "ResourceNode.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Crafting/CraftingMachine.h"
#include "AutomationFoundation/Inventory/InventoryComponent.h"

AResourceExtractor::AResourceExtractor()
{
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Resource Extractor Output Inventory"));
	Inventory->SetInventorySize(1);
}

void AResourceExtractor::OnOutputItemRemoved(UInventoryItemInstance* OldItem, int32 ItemIndex)
{
}

UInventoryComponent* AResourceExtractor::GetProviderInventory_Implementation()
{
	return Inventory;
}

UInventoryComponent* AResourceExtractor::GetAcceptorInventory_Implementation()
{
	return Inventory;
}

bool AResourceExtractor::IsPlaceable_Implementation()
{
	const FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, 200.f);
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, GetActorLocation(), ECC_Visibility))
	{
		return Cast<AResourceNode>(HitResult.GetActor()) != nullptr;
	}
	return false;
}

void AResourceExtractor::UpdateLocation_Implementation(const FHitResult& HitResult)
{
	if (AResourceNode* ResourceVein = Cast<AResourceNode>(HitResult.GetActor()))
	{
		SetActorLocation(ResourceVein->GetActorLocation());
	}
	else
	{
		Super::UpdateLocation_Implementation(HitResult);
	}

	HologramMaterial->SetVectorParameterValue(TEXT("Color"), IsPlaceable() ? FColor::Green : FColor::Red);
}

void AResourceExtractor::CreatePreview_Implementation()
{
	Super::CreatePreview_Implementation();

	SetAllMaterialOverlays(HologramMaterial);
}

void AResourceExtractor::ScrollPreview_Implementation(float Value)
{
	const FRotator NewRotation = GetActorRotation() + FRotator(0.0f, 45.0f * Value, 0.0f);
	SetActorRotation(NewRotation);
}

void AResourceExtractor::OnPreviewPlaced_Implementation()
{
	SetAllMaterialOverlays(nullptr, true);

	// We need to get the recipe from the vein we are on
	const FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, 200.f);
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, GetActorLocation(), ECC_Visibility))
	{
		if (const AResourceNode* Node = Cast<AResourceNode>(HitResult.GetActor()))
		{
			UpdateRecipe(Node->GetHarvestedResource());
			StartCrafting();
			return;
		}
	}
	LOG_ERROR(LogTemp, "Did not find Ore Vein To Extract from");
}

bool AResourceExtractor::CanOutput_Implementation()
{
	const UInventoryComponent* InventoryComponent = IItemProvider::Execute_GetProviderInventory(this);
	return Super::CanOutput_Implementation() && !InventoryComponent->IsEmpty();
}
