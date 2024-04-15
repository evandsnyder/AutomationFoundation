#include "Machine.h"

#include "ConveyorBelt.h"
#include "AutomationFoundation/BuildSystem/AttachPoint.h"
#include "AutomationFoundation/BuildSystem/ConveyorItem.h"
#include "AutomationFoundation/Inventory/InventoryItemInstance.h"
#include "Kismet/GameplayStatics.h"

AMachine::AMachine()
{
}

void AMachine::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TArray<AActor*> ChildActors;
	GetAllChildActors(ChildActors);

	for (AActor* Actor : ChildActors)
	{
		if (AAttachPoint* AttachPoint = Cast<AAttachPoint>(Actor))
		{
			AttachPoint->SetParentBuild(this);
		}
	}
}

void AMachine::ConnectAttachPointAsInput(AAttachPoint* AttachPoint)
{
	// TODO: Implement
}

void AMachine::ConnectAttachPointAsOutput(AAttachPoint* AttachPoint)
{
	if (AConveyorBelt* Conveyor = Cast<AConveyorBelt>(AttachPoint->GetConnectedBuild()))
	{
		float& Interval = Outputs.FindOrAdd(AttachPoint);
		Interval = ItemSpawnInterval / Conveyor->GetSpeed();
		StartOutputting();
	}
}

void AMachine::StartOutputting_Implementation()
{
}

bool AMachine::OutputFromAttachPoint(AAttachPoint* OutputAttachPoint, UItemSpecification* ItemSpecification)
{
	if (!CanOutput())
	{
		return false;
	}

	if (AConveyorBelt* ConveyorBelt = Cast<AConveyorBelt>(OutputAttachPoint->GetConnectedBuild()))
	{
		if (ConveyorBelt->CanSpawnNextItem())
		{
			const FTransform SpawnTransform = ConveyorBelt->GetTransformByAlpha(0.0f);

			AConveyorItem* NewItem = GetWorld()->SpawnActorDeferred<AConveyorItem>(AConveyorItem::StaticClass(), SpawnTransform);
			if (NewItem)
			{
				NewItem->SetConveyor(ConveyorBelt);
				NewItem->SetItemSpecification(ItemSpecification);
				NewItem->SetItemInterval(ItemSpawnInterval);

				UGameplayStatics::FinishSpawningActor(NewItem, SpawnTransform);
				NewItem->OnSpawnNext.AddDynamic(this, &AMachine::StartOutputting);
				return true;
			}
		}
	}

	return false;
}

bool AMachine::AcceptInput(AAttachPoint* ProvidingAttachPoint, AConveyorItem* IncomingItem)
{
	IncomingItem->Destroy();
	return true;
}

bool AMachine::CanOutput_Implementation()
{
	return true;
}

bool AMachine::CanAcceptInput(AAttachPoint* InputAttachPoint, AConveyorItem* IncomingItem)
{
	if (!Inputs.Contains(InputAttachPoint))
	{
		// Shouldn't really happen... it means we are trying to accept an input from some unconnected output
		return false;
	}

	UInventoryItemInstance* ItemInstance = Inputs[InputAttachPoint];
	if (nullptr == IncomingItem->GetItemSpecification())
	{
		return false;
	}

	if (IsValid(ItemInstance) && ItemInstance->ItemSpecification->ItemID != IncomingItem->GetItemSpecification()->ItemID)
	{
		return false;
	}

	return ItemInstance->CurrentStackSize < ItemInstance->ItemSpecification->MaxStackSize;
}
