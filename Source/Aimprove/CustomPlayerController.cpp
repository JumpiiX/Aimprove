#include "CustomPlayerController.h"
#include "TopdownCameraPawn.h"
#include "UMainHUD.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryWidget.h"

ACustomPlayerController::ACustomPlayerController()
{
    bIsInTopdownMode = true;
    bShowMouseCursor = true;
    CurrentMouseCursor = EMouseCursor::Crosshairs;
}

void ACustomPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("CustomPlayerController: BeginPlay is called!"));

    // Initialize MainHUD
    if (!MainHUD && MainHUDClass)
    {
        MainHUD = CreateWidget<UMainHUD>(this, MainHUDClass);
        if (MainHUD)
        {
            MainHUD->AddToViewport(1); // Lower Z-order
            MainHUD->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("MainHUD added to viewport."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create MainHUD instance."));
        }
    }

    if (!InventoryWidget && InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
        if (InventoryWidget)
        {
            InventoryWidget->AddToViewport(2); // Higher Z-order
            InventoryWidget->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("InventoryWidget added to viewport."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create InventoryWidget instance."));
        }
    }


    // Initialize TopdownCameraPawn
    TopdownPawn = Cast<ATopdownCameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCameraPawn::StaticClass()));
    if (TopdownPawn)
    {
        Possess(TopdownPawn);
        bIsInTopdownMode = true;

        bShowMouseCursor = true;
        CurrentMouseCursor = EMouseCursor::Crosshairs;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No TopdownCameraPawn found in the level!"));
    }
}

void ACustomPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ACustomPlayerController::SwitchCameraMode);
}

void ACustomPlayerController::SwitchCameraMode()
{
    UClass* CharacterBlueprintClass = LoadObject<UClass>(nullptr, TEXT("/Game/Blueprints/CBP_SandboxCharacter.CBP_SandboxCharacter_C"));
    if (!CharacterBlueprintClass) return;

    AActor* FoundCharacter = UGameplayStatics::GetActorOfClass(GetWorld(), CharacterBlueprintClass);
    ACharacter* SandboxCharacter = Cast<ACharacter>(FoundCharacter);

    if (SandboxCharacter)
    {
        if (bIsInTopdownMode)
        {
            Possess(SandboxCharacter);
            bIsInTopdownMode = false;
            bShowMouseCursor = false;

            FInputModeGameOnly GameOnlyMode;
            SetInputMode(GameOnlyMode);
        }
        else
        {
            if (TopdownPawn)
            {
                Possess(TopdownPawn);
                bIsInTopdownMode = true;
                bShowMouseCursor = true;

                FInputModeGameAndUI GameAndUIMode;
                GameAndUIMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                SetInputMode(GameAndUIMode);
            }
        }
    }
}
