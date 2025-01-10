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

   LevelBlockClass = LoadClass<AActor>(nullptr, TEXT("/Game/LevelPrototyping/LevelBlock_Traversable.LevelBlock_Traversable_C"));
    WallClass = LoadClass<AActor>(nullptr, TEXT("/Game/LevelPrototyping/wall_Traversable.wall_Traversable_C"));   
   if (LevelBlockClass && WallClass)
   {
       UE_LOG(LogTemp, Warning, TEXT("Building classes loaded successfully"));
   }

    if (block)
       block->SetVisibility(ESlateVisibility::Visible);
   if (wall)
       wall->SetVisibility(ESlateVisibility::Visible);

   SetVisibility(ESlateVisibility::Visible);
   SetIsFocusable(true);
   
   if (APlayerController* PC = GetOwningPlayer())
   {
       PC->InputComponent->BindAction("Rotate", IE_Pressed, this, &UInventoryWidget::OnRotatePressed);
       PC->CurrentMouseCursor = EMouseCursor::Crosshairs;
       PC->bShowMouseCursor = true;
   }
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        // Check block drag
        if (block)
        {
            FGeometry BlockGeometry = block->GetCachedGeometry();
            FVector2D LocalPos = BlockGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
            FVector2D Size = BlockGeometry.GetLocalSize();
            
            if (LocalPos.X >= 0 && LocalPos.X <= Size.X && LocalPos.Y >= 0 && LocalPos.Y <= Size.Y)
            {
                BlockToSpawn = LevelBlockClass;
                return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
            }
        }
        
        // Check wall drag
        if (wall)  
        {
            FGeometry WallGeometry = wall->GetCachedGeometry();
            FVector2D LocalPos = WallGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
            FVector2D Size = WallGeometry.GetLocalSize();
    
            UE_LOG(LogTemp, Warning, TEXT("Wall bounds check - LocalPos: %s, Size: %s"), 
                *LocalPos.ToString(), *Size.ToString());
            
            if (LocalPos.X >= 0 && LocalPos.X <= Size.X && LocalPos.Y >= 0 && LocalPos.Y <= Size.Y)
            {
                UE_LOG(LogTemp, Warning, TEXT("Wall drag detected!"));
                BlockToSpawn = WallClass;
                return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
            }
        }
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
                       TopdownPawn->UpdatePreviewLocation(HitResult.ImpactPoint, BlockToSpawn);
                   }
               }
           }
       }
   }
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
                   TopdownPawn->UpdatePreviewLocation(HitResult.ImpactPoint, BlockToSpawn);
                   return true;
               }
           }
       }
   }
   return false;
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
   if (ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(GetOwningPlayerPawn()))
   {
       APlayerController* PC = GetOwningPlayer();
       if (!PC) return false;

       FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();
       FVector WorldPosition, WorldDirection;
       
       if (PC->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldPosition, WorldDirection))
       {
           return TopdownPawn->HandleBlockPlacement(WorldPosition, WorldDirection, BlockToSpawn);
       }
   }
   return false;
}

void UInventoryWidget::OnRotatePressed()
{
    if (ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(GetOwningPlayerPawn()))
    {
        if (TopdownPawn->PreviewBlock)
        {
            CurrentRotation += 90.0f;
            if (CurrentRotation >= 360.0f)
                CurrentRotation = 0.0f;
            TopdownPawn->PreviewRotation = CurrentRotation;
            TopdownPawn->PreviewBlock->SetActorRotation(FRotator(0.0f, CurrentRotation, 0.0f));
        }
    }
}