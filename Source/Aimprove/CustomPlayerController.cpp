// CustomPlayerController.cpp
#include "CustomPlayerController.h"
#include "TopdownCameraPawn.h"
#include "GameFramework/Character.h"

ACustomPlayerController::ACustomPlayerController()
{
    bIsInTopdownMode = true; // Start in Top-Down mode
    bShowMouseCursor = true; // Always show cursor
    CurrentMouseCursor = EMouseCursor::Crosshairs; // Use crosshair cursor
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
        
        // Ensure mouse cursor is visible
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

    // Bind switching camera mode to the "M" key
    InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ACustomPlayerController::SwitchCameraMode);
}

void ACustomPlayerController::SwitchCameraMode()
{
    // Get the Blueprint class reference
    UClass* CharacterBlueprintClass = LoadClass<ACharacter>(nullptr, TEXT("/Game/Blueprints/CBP_SandboxCharacter.CBP_SandboxCharacter_C"));
    
    if (CharacterBlueprintClass)
    {
        // Find character of this blueprint class
        AActor* FoundCharacter = UGameplayStatics::GetActorOfClass(GetWorld(), CharacterBlueprintClass);
        ACharacter* SandboxCharacter = Cast<ACharacter>(FoundCharacter);

        UE_LOG(LogTemp, Warning, TEXT("SwitchCameraMode called"));
        UE_LOG(LogTemp, Warning, TEXT("Current mode is Topdown: %s"), bIsInTopdownMode ? TEXT("true") : TEXT("false"));
        
        if (SandboxCharacter)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found SandboxCharacter"));
            
            if (bIsInTopdownMode)
            {
                Possess(SandboxCharacter);
                bIsInTopdownMode = false;
                bShowMouseCursor = false; // Hide cursor in third person mode
                UE_LOG(LogTemp, Warning, TEXT("Successfully switched to Sandbox Character"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("SandboxCharacter not found in level!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not find SandboxCharacter Blueprint class!"));
    }
}