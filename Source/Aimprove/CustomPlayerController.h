#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "CustomPlayerController.generated.h"

UCLASS()
class AIMPROVE_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Current Camera Mode
	bool bIsInTopdownMode;

	// Switching Function
	void SwitchCameraMode();

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	ACustomPlayerController();

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
};