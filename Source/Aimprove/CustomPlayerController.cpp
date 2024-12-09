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

	// Possess the TopdownCameraPawn at the start
	ATopdownCameraPawn* TopdownPawn = GetWorld()->SpawnActor<ATopdownCameraPawn>();
	if (TopdownPawn)
	{
		Possess(TopdownPawn);
	}
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind switching camera mode to the "O" key
	InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ACustomPlayerController::SwitchCameraMode);
}

void ACustomPlayerController::SwitchCameraMode()
{
	if (bIsInTopdownMode)
	{
		// Switch to Third-Person Character
		ACharacter* ThirdPersonCharacter = GetWorld()->SpawnActor<ACharacter>();
		if (ThirdPersonCharacter)
		{
			Possess(ThirdPersonCharacter);
			bIsInTopdownMode = false;
		}
	}
	else
	{
		// Switch back to Top-Down Camera
		ATopdownCameraPawn* TopdownPawn = GetWorld()->SpawnActor<ATopdownCameraPawn>();
		if (TopdownPawn)
		{
			Possess(TopdownPawn);
			bIsInTopdownMode = true;
		}
	}
}
