#include "CustomPlayerController.h"
#include "TopdownCameraPawn.h"
#include "GameFramework/Character.h"

ACustomPlayerController::ACustomPlayerController()
{
    bIsInTopdownMode = true;
}

void ACustomPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Find the existing TopdownCameraPawn in the level
    ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCameraPawn::StaticClass()));

    if (TopdownPawn)
    {
        // Possess the TopdownCameraPawn
        Possess(TopdownPawn);
        bIsInTopdownMode = true;
        UE_LOG(LogTemp, Warning, TEXT("Successfully possessed TopdownCameraPawn"));
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
    // Format is "/Game/[FolderPath]/[BlueprintName].[BlueprintName]_C"
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