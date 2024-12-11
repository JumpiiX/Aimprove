// InventoryDragDropOperation.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

UCLASS()
class AIMPROVE_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	// Reference to the block class we want to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AActor> BlockClass;
};