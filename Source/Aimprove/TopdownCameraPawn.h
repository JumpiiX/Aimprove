// TopdownCameraPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "TopdownCameraPawn.generated.h"

UCLASS()
class AIMPROVE_API ATopdownCameraPawn : public APawn  // Changed from YOURPROJECT_API to AIMPROVE_API
{
	GENERATED_BODY()

public:
	ATopdownCameraPawn();

protected:
	// Camera Component reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* TopdownCameraComponent;

	virtual void BeginPlay() override;

public:
	// Getter for the camera component
	FORCEINLINE UCameraComponent* GetTopdownCamera() const { return TopdownCameraComponent; }
};