#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMainHUD.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "InventoryWidget.generated.h"

class UDragDropOperation;
struct FGeometry;
struct FPointerEvent;

UCLASS()
class AIMPROVE_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION()
    void OnRotatePressed();

    // Costs for items
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
    int32 BlockCost = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
    int32 WallCost = 50;

    bool CanAffordItem(TSubclassOf<AActor> ItemClass) const;
    void PayForItem(TSubclassOf<AActor> ItemClass);

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ErrorMessageText;

    UPROPERTY(meta = (BindWidget))
    UBorder* ErrorMessageBorder;

    void ShowNotEnoughCoinsMessage();
    void FlashRed(UWidget* Widget);  // Neue Funktion für den Farbwechsel

protected:
    virtual void NativeConstruct() override;
    virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
   
    UPROPERTY(meta = (BindWidget))
    class UImage* block;
   
    UPROPERTY(meta = (BindWidget))
    class UImage* wall;
   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag Drop")
    bool bIsDragDropEnabled = true;
   
    UPROPERTY()
    TSubclassOf<AActor> LevelBlockClass;
   
    UPROPERTY()
    TSubclassOf<AActor> WallClass;
   
    UPROPERTY()
    TSubclassOf<AActor> BlockToSpawn;

    UPROPERTY()
    float CurrentRotation = 0.0f;

    FTimerHandle ColorResetTimer;
    FLinearColor OriginalColor;   
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
