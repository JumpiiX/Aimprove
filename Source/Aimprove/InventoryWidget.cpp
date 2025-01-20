// InventoryWidget.cpp
#include "InventoryWidget.h"

#include "CustomPlayerController.h"
#include "TopdownCameraPawn.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InventoryDragDropOperation.h"
#include "Kismet/GameplayStatics.h"
#include "UMainHUD.h"
#include "Components/TextBlock.h"

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
        // Block check
        if (block)
        {
            FGeometry BlockGeometry = block->GetCachedGeometry();
            FVector2D LocalPos = BlockGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
            FVector2D Size = BlockGeometry.GetLocalSize();
            
            if (LocalPos.X >= 0 && LocalPos.X <= Size.X && LocalPos.Y >= 0 && LocalPos.Y <= Size.Y)
            {
                BlockToSpawn = LevelBlockClass;
                if (!CanAffordItem(BlockToSpawn))
                {
                    ShowNotEnoughCoinsMessage();
                    return FReply::Unhandled();
                }
                return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
            }
        }
        
        // Wall check
        if (wall)  
        {
            FGeometry WallGeometry = wall->GetCachedGeometry();
            FVector2D LocalPos = WallGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
            FVector2D Size = WallGeometry.GetLocalSize();
            
            if (LocalPos.X >= 0 && LocalPos.X <= Size.X && LocalPos.Y >= 0 && LocalPos.Y <= Size.Y)
            {
                BlockToSpawn = WallClass;
                if (!CanAffordItem(BlockToSpawn))
                {
                    ShowNotEnoughCoinsMessage();
                    return FReply::Unhandled();
                }
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
    // Erst prüfen ob wir uns das Item leisten können
    if (!CanAffordItem(BlockToSpawn))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot afford item! Required coins not available."));
        return false;
    }

    if (ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(GetOwningPlayerPawn()))
    {
        APlayerController* PC = GetOwningPlayer();
        if (!PC) return false;

        FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();
        FVector WorldPosition, WorldDirection;
        
        if (PC->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldPosition, WorldDirection))
        {
            // Wenn das Platzieren erfolgreich war, ziehe die Kosten ab
            bool PlacementSuccess = TopdownPawn->HandleBlockPlacement(WorldPosition, WorldDirection, BlockToSpawn);
            if (PlacementSuccess)
            {
                PayForItem(BlockToSpawn);
                UE_LOG(LogTemp, Warning, TEXT("Item placed and paid for successfully."));
                return true;
            }
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

bool UInventoryWidget::CanAffordItem(TSubclassOf<AActor> ItemClass) const
{
    ACustomPlayerController* PC = Cast<ACustomPlayerController>(GetOwningPlayer());
    if (!PC || !PC->MainHUD) return false;

    int32 CurrentCoins = FCString::Atoi(*PC->MainHUD->CoinText->GetText().ToString());
    
    if (ItemClass == LevelBlockClass)
    {
        return CurrentCoins >= BlockCost;
    }
    else if (ItemClass == WallClass)
    {
        return CurrentCoins >= WallCost;
    }
    return false;
}

void UInventoryWidget::PayForItem(TSubclassOf<AActor> ItemClass)
{
    ACustomPlayerController* PC = Cast<ACustomPlayerController>(GetOwningPlayer());
    if (!PC || !PC->MainHUD) return;

    int32 CurrentCoins = FCString::Atoi(*PC->MainHUD->CoinText->GetText().ToString());
    int32 Cost = 0;

    if (ItemClass == LevelBlockClass)
    {
        Cost = BlockCost;
    }
    else if (ItemClass == WallClass)
    {
        Cost = WallCost;
    }

    PC->MainHUD->UpdateCoinCount(CurrentCoins - Cost);
}


void UInventoryWidget::ShowNotEnoughCoinsMessage()
{
    // Text-Nachricht anzeigen
    if (ErrorMessageText && ErrorMessageBorder)
    {
        ErrorMessageText->SetText(FText::FromString("Not enough coins!"));
        ErrorMessageBorder->SetVisibility(ESlateVisibility::Visible);

        // Nach 2 Sekunden ausblenden
        FTimerHandle MessageTimer;
        GetWorld()->GetTimerManager().SetTimer(MessageTimer, [this]()
        {
            if (ErrorMessageBorder)
            {
                ErrorMessageBorder->SetVisibility(ESlateVisibility::Hidden);
            }
        }, 2.0f, false);
    }

    // Item rot blinken lassen
    UImage* ItemToFlash = nullptr;
    if (BlockToSpawn == LevelBlockClass)
    {
        ItemToFlash = block;
    }
    else if (BlockToSpawn == WallClass)
    {
        ItemToFlash = wall;
    }

    if (ItemToFlash)
    {
        // Originale Farbe speichern
        FLinearColor OriginalTint = ItemToFlash->ColorAndOpacity;
        
        // Helles, kräftiges Rot
        ItemToFlash->SetColorAndOpacity(FLinearColor(1.0f, 0.2f, 0.2f, 1.0f));

        // Zurück zur Original-Farbe
        FTimerHandle ColorTimer;
        GetWorld()->GetTimerManager().SetTimer(ColorTimer, [this, ItemToFlash, OriginalTint]()
        {
            if (ItemToFlash)
            {
                ItemToFlash->SetColorAndOpacity(OriginalTint);
            }
        }, 0.3f, false);
    }
}
void UInventoryWidget::FlashRed(UWidget* Widget)
{
    if (!Widget) return;

    UBorder* Border = Cast<UBorder>(Widget);
    if (Border)
    {
        // Für Border
        OriginalColor = Border->GetBrushColor();
        Border->SetBrushColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));  // Rot

        GetWorld()->GetTimerManager().SetTimer(ColorResetTimer, [this, Border]()
        {
            if (Border)
            {
                Border->SetBrushColor(OriginalColor);
            }
        }, 0.3f, false);
    }
    else if (UTextBlock* Text = Cast<UTextBlock>(Widget))
    {
        // Für TextBlock
        FSlateColor CurrentColor = Text->GetColorAndOpacity();
        OriginalColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Default weiß
        Text->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)));  // Rot

        GetWorld()->GetTimerManager().SetTimer(ColorResetTimer, [this, Text]()
        {
            if (Text)
            {
                Text->SetColorAndOpacity(FSlateColor(OriginalColor));
            }
        }, 0.3f, false);
    }
}