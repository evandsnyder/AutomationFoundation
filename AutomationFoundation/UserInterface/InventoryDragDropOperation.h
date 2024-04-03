#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

class UInventoryComponent;

UCLASS()
class UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UInventoryComponent* SourceInventory;

	UPROPERTY()
	int32 ItemIndex;
};
