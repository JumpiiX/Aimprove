#include "TopdownCameraPawn.h"

// Sets default values
ATopdownCameraPawn::ATopdownCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// SpringArm Component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 800.0f; // Distance from the Pawn
	SpringArmComponent->bDoCollisionTest = false; // Disable collision for smooth camera movement

	// Camera Component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ATopdownCameraPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATopdownCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Bind input functions
void ATopdownCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUp", this, &ATopdownCameraPawn::MoveCameraUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATopdownCameraPawn::MoveCameraRight);
}

void ATopdownCameraPawn::MoveCameraUp(float Value)
{
	if (Controller && Value != 0.0f)
	{
		AddMovementInput(FVector::ForwardVector, Value);
	}
}

void ATopdownCameraPawn::MoveCameraRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		AddMovementInput(FVector::RightVector, Value);
	}
}
