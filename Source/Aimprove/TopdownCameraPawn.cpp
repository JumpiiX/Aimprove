#include "TopdownCameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATopdownCameraPawn::ATopdownCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root Component
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

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

// Prevent input for this pawn
void ATopdownCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// No input bindings
}