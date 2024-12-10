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

	// Spawn und Possess den TopdownCameraPawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ATopdownCameraPawn* TopdownPawn = GetWorld()->SpawnActor<ATopdownCameraPawn>(ATopdownCameraPawn::StaticClass(), SpawnParams);
	if (TopdownPawn)
	{
		Possess(TopdownPawn);
		UE_LOG(LogTemp, Warning, TEXT("Possessed TopdownCameraPawn successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn TopdownCameraPawn"));
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
	if (bIsInTopdownMode)
	{
		// Switch to Third-Person Character
		ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
		if (ControlledCharacter)
		{
			// Besitze den existierenden Third-Person Charakter
			Possess(ControlledCharacter);
			bIsInTopdownMode = false;
			UE_LOG(LogTemp, Warning, TEXT("Switched to Third-Person View"));
		}
	}
	else
	{
		// Switch back to Top-Down Camera
		ATopdownCameraPawn* TopdownPawn = Cast<ATopdownCameraPawn>(GetPawn());
		if (TopdownPawn)
		{
			// Besitze die existierende Top-Down Kamera
			Possess(TopdownPawn);
			bIsInTopdownMode = true;
			UE_LOG(LogTemp, Warning, TEXT("Switched to Top-Down View"));
		}
	}
}
