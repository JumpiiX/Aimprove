// InventoryWidget.cpp
#include "InventoryWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InventoryDragDropOperation.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (DraggableSquare)
    {
        DraggableSquare->SetVisibility(ESlateVisibility::Visible);
    }

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->CurrentMouseCursor = EMouseCursor::Crosshairs;
        PC->bShowMouseCursor = true;
    }
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
        return Reply.NativeReply;
    }

    return FReply::Unhandled();
}

void UInventoryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    if (!BlockToSpawn) return;

    UInventoryDragDropOperation* DragDropOp = Cast<UInventoryDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UInventoryDragDropOperation::StaticClass()));
    
    if (DragDropOp)
    {
        DragDropOp->BlockClass = BlockToSpawn;
        DragDropOp->DefaultDragVisual = this;  // Use the whole widget as drag visual
        DragDropOp->Pivot = EDragPivot::CenterCenter;  // Center the drag visual
        
        OutOperation = DragDropOp;

        if (APlayerController* PC = GetOwningPlayer())
        {
            PC->CurrentMouseCursor = EMouseCursor::Crosshairs;
            PC->bShowMouseCursor = true;
        }
    }
}