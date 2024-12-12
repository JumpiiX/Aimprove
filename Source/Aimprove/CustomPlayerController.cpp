// CustomPlayerController.cpp
#include "CustomPlayerController.h"
#include "TopdownCameraPawn.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"

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

    ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCameraPawn::StaticClass()));

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
    UClass* CharacterBlueprintClass = LoadClass<ACharacter>(nullptr, TEXT("/Game/Blueprints/CBP_SandboxCharacter.CBP_SandboxCharacter_C"));
    
    if (CharacterBlueprintClass)
    {
        AActor* FoundCharacter = UGameplayStatics::GetActorOfClass(GetWorld(), CharacterBlueprintClass);
        ACharacter* SandboxCharacter = Cast<ACharacter>(FoundCharacter);

        if (SandboxCharacter)
        {
            if (bIsInTopdownMode)
            {
                // Wechsel zu Third Person
                ATopdownCameraPawn* CurrentTopdown = Cast<ATopdownCameraPawn>(GetPawn());
                if (CurrentTopdown && CurrentTopdown->InventoryWidgetInstance)
                {
                    CurrentTopdown->InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
                    CurrentTopdown->DestroyPreviewBlock();
                }
                
                Possess(SandboxCharacter);
                bIsInTopdownMode = false;
                bShowMouseCursor = false;
                
                FInputModeGameOnly GameOnlyMode;
                SetInputMode(GameOnlyMode);
            }
            else
            {
                // Wechsel zurück zu Topdown
                ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCameraPawn::StaticClass()));
                if (TopdownPawn)
                {
                    Possess(TopdownPawn);
                    bIsInTopdownMode = true;
                    bShowMouseCursor = true;
                    CurrentMouseCursor = EMouseCursor::Crosshairs;

                    FInputModeGameAndUI GameAndUIMode;
                    GameAndUIMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                    SetInputMode(GameAndUIMode);

                    if (TopdownPawn->InventoryWidgetInstance)
                    {
                        TopdownPawn->InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);
                    }
                }
            }
        }
    }
}