// TopdownCameraPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "InventoryWidget.h"
#include "TopdownCameraPawn.generated.h"

UCLASS()
class AIMPROVE_API ATopdownCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ATopdownCameraPawn();

protected:
	// Camera Component reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* TopdownCameraComponent;

	// The widget instance
	UPROPERTY()
	UInventoryWidget* InventoryWidgetInstance;
	void CreateInventoryWidget();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// Getter for the camera component
	FORCEINLINE UCameraComponent* GetTopdownCamera() const { return TopdownCameraComponent; }
};