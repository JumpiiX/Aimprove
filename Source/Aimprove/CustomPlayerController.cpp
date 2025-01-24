#include "CustomPlayerController.h"
#include "TopdownCameraPawn.h"
#include "UMainHUD.h"
#include "SandboxCharacter.h"  // Include full definition of SandboxCharacter
#include "Kismet/GameplayStatics.h"
#include "InventoryWidget.h"
#include "GameFramework/PlayerStart.h"

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
            MainHUD->AddToViewport(1);
            MainHUD->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("MainHUD added to viewport."));
        }
    }

    // Initialize InventoryWidget
    if (!InventoryWidget && InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
        if (InventoryWidget)
        {
            InventoryWidget->AddToViewport(2);
            UpdateInventoryVisibility(bIsInTopdownMode);
            UE_LOG(LogTemp, Warning, TEXT("InventoryWidget added to viewport with initial visibility."));
        }
    }

    SandboxCharacter = Cast<ASandboxCharacter>(GetPawn());
    if (!SandboxCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("CustomPlayerController: Could not cast Pawn to SandboxCharacter."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CustomPlayerController: SandboxCharacter set to %s"), *SandboxCharacter->GetName());
    }


    // Find and possess the TopdownCameraPawn
    TopdownPawn = Cast<ATopdownCameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCameraPawn::StaticClass()));
    if (TopdownPawn)
    {
        Possess(TopdownPawn);
        bIsInTopdownMode = true;
        bShowMouseCursor = true;
        CurrentMouseCursor = EMouseCursor::Crosshairs;
        UE_LOG(LogTemp, Warning, TEXT("PlayerController now possesses TopdownCameraPawn: %s"), *TopdownPawn->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not find TopdownCameraPawn to possess."));
    }
}

void ACustomPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ACustomPlayerController::SwitchCameraMode);
}

void ACustomPlayerController::UpdateInventoryVisibility(bool bShouldBeVisible)
{
    if (InventoryWidget)
    {
        InventoryWidget->SetVisibility(bShouldBeVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        UE_LOG(LogTemp, Warning, TEXT("Inventory visibility set to: %s"), bShouldBeVisible ? TEXT("Visible") : TEXT("Hidden"));
    }
}

// CustomPlayerController.cpp - Überarbeitete SwitchCameraMode
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
            // Switching to Third Person mode
            Possess(SandboxCharacter);
            bIsInTopdownMode = false;
            bShowMouseCursor = false;

            // Explicitly hide inventory
            UpdateInventoryVisibility(false);

            FInputModeGameOnly GameOnlyMode;
            SetInputMode(GameOnlyMode);
        }
        else
        {
            // Switching back to Topdown mode
            if (TopdownPawn)
            {
                Possess(TopdownPawn);
                bIsInTopdownMode = true;
                bShowMouseCursor = true;

                // Show inventory
                UpdateInventoryVisibility(true);

                FInputModeGameAndUI GameAndUIMode;
                GameAndUIMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                SetInputMode(GameAndUIMode);
            }
        }
    }
}

void ACustomPlayerController::RestartRound()
{
    AActor* StartPoint = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
    if (StartPoint)
    {
        // Reset character position first
        APawn* CurrentPawn = GetPawn();
        if (CurrentPawn)
        {
            CurrentPawn->SetActorLocation(StartPoint->GetActorLocation());
            CurrentPawn->SetActorRotation(StartPoint->GetActorRotation());
        }
    }
    
    // Then switch to topdown view
    ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCameraPawn::StaticClass()));
    if (TopdownPawn)
    {
        Possess(TopdownPawn);
        bIsInTopdownMode = true;
        bShowMouseCursor = true;
        CurrentMouseCursor = EMouseCursor::Crosshairs;

        UpdateInventoryVisibility(true);
        
        FInputModeGameAndUI GameAndUIMode;
        GameAndUIMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(GameAndUIMode);
    }
}