#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TopdownCameraPawn.generated.h"

UCLASS()
class AIMPROVE_API ATopdownCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATopdownCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Camera and SpringArm
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// Camera movement functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void ZoomCamera(float Value);

	// Camera movement speed
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MovementSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinZoom = 400.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxZoom = 1200.f;
};
