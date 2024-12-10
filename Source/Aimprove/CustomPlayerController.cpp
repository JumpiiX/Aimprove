#include "CustomPlayerController.h"
#include "TopdownCameraPawn.h"
#include "GameFramework/Character.h"

ACustomPlayerController::ACustomPlayerController()
{
    bIsInTopdownMode = true; // Start in Top-Down mode
}

void ACustomPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("CustomPlayerController: BeginPlay is called!"));

    // Find the existing TopdownCameraPawn in the level
    ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCameraPawn::StaticClass()));

    if (TopdownPawn)
    {
        // Possess the TopdownCameraPawn
        Possess(TopdownPawn);
        bIsInTopdownMode = true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No TopdownCameraPawn found in the level!"));
    }
}

void ACustomPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Bind switching camera mode to the "M" key
    InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ACustomPlayerController::SwitchCameraMode);
}

void ACustomPlayerController::SwitchCameraMode()
{
    ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
    ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCameraPawn::StaticClass()));

    if (ControlledCharacter && TopdownPawn)
    {
        if (bIsInTopdownMode)
        {
            // Switch to Third-Person Character
            Possess(ControlledCharacter);
            bIsInTopdownMode = false;
            UE_LOG(LogTemp, Warning, TEXT("Switched to Third-Person View"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Unable to switch camera - missing pawn or character"));
    }
}