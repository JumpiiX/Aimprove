#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "CustomPlayerController.generated.h"


/**
 * Custom player controller to handle HUD and camera functionality.
 */
UCLASS()
class AIMPROVE_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACustomPlayerController();
	UPROPERTY()
	class UMainHUD* MainHUD;

	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass; // Class reference for the Inventory Widget

	UPROPERTY()
	UInventoryWidget* InventoryWidget; // Instance of the Inventory Widget
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UMainHUD> MainHUDClass;

private:
	void SwitchCameraMode();

	bool bIsInTopdownMode;

	/** Reference to the top-down camera */
	UPROPERTY()
	class ATopdownCameraPawn* TopdownPawn;

	/** Reference to the main HUD */
	
};
