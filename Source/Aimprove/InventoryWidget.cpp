// InventoryWidget.cpp
#include "InventoryWidget.h"
#include "TopdownCameraPawn.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InventoryDragDropOperation.h"
#include "Kismet/GameplayStatics.h"

// InventoryWidget.cpp - Aktualisiere NativeConstruct
void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Load the LevelBlock class
    BlockToSpawn = LoadClass<AActor>(nullptr, TEXT("/Game/LevelPrototyping/LevelBlock_Traversable.LevelBlock_Traversable_C"));
    
    if (BlockToSpawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("LevelBlock class loaded successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load LevelBlock class"));
    }

    if (DraggableSquare)
    {
        DraggableSquare->SetVisibility(ESlateVisibility::Visible);
    }

    // Add these lines
    SetVisibility(ESlateVisibility::Visible);
    SetIsFocusable(true);
    
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
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
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
        DragDropOp->Pivot = EDragPivot::MouseDown;
        OutOperation = DragDropOp;
        
        // Start Preview
        if (ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(GetOwningPlayerPawn()))
        {
            FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
            FVector WorldPosition, WorldDirection;
            
            if (APlayerController* PC = GetOwningPlayer())
            {
                if (PC->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldPosition, WorldDirection))
                {
                    FHitResult HitResult;
                    if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, WorldPosition + (WorldDirection * 10000.0f), ECC_Visibility))
                    {
                        TopdownPawn->UpdatePreviewLocation(HitResult.ImpactPoint);
                    }
                }
            }
        }
    }
}
bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop called"));

    if (ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(GetOwningPlayerPawn()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Found TopdownPawn"));
        
        APlayerController* PC = GetOwningPlayer();
        if (!PC)
        {
            UE_LOG(LogTemp, Error, TEXT("No PlayerController found"));
            return false;
        }

        FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();
        FVector WorldPosition, WorldDirection;
        
        if (PC->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldPosition, WorldDirection))
        {
            UE_LOG(LogTemp, Warning, TEXT("Deproject successful. WorldPos: %s"), *WorldPosition.ToString());
            return TopdownPawn->HandleBlockPlacement(WorldPosition, WorldDirection);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Deproject failed"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not find TopdownPawn"));
    }
    return false;
}

bool UInventoryWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(GetOwningPlayerPawn()))
    {
        FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();
        FVector WorldPosition, WorldDirection;
        
        if (APlayerController* PC = GetOwningPlayer())
        {
            if (PC->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldPosition, WorldDirection))
            {
                FHitResult HitResult;
                if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, WorldPosition + (WorldDirection * 10000.0f), ECC_Visibility))
                {
                    TopdownPawn->UpdatePreviewLocation(HitResult.ImpactPoint);
                    return true;
                }
            }
        }
    }
    return false;
}