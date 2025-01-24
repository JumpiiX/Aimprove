#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

// Forward declaration of ASandboxCharacter
class ASandboxCharacter;
class UInventoryWidget;
class ATopdownCameraPawn;

/**
 * Custom player controller to handle HUD and camera functionality.
 */
UCLASS()
class AIMPROVE_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACustomPlayerController();

	ASandboxCharacter* GetSandboxCharacter() const { return SandboxCharacter; }

	void UpdateInventoryVisibility(bool bShouldBeVisible);

	UPROPERTY()
	class UMainHUD* MainHUD;

	void RestartRound();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UMainHUD> MainHUDClass;

private:
	void SwitchCameraMode();

	bool bIsInTopdownMode;

	/** Reference to the top-down camera */
	UPROPERTY()
	ATopdownCameraPawn* TopdownPawn;

	/** Pointer to the SandboxCharacter */
	UPROPERTY()
	ASandboxCharacter* SandboxCharacter;
};
