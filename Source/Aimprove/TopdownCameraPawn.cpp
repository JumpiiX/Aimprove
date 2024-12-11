// TopdownCameraPawn.cpp
#include "TopdownCameraPawn.h"

ATopdownCameraPawn::ATopdownCameraPawn()
{
	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create camera component only if it doesn't exist
	TopdownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopdownCamera"));
	if (TopdownCameraComponent)
	{
		TopdownCameraComponent->SetupAttachment(RootComponent);
		// We don't set rotation/location here since it's already set in the level
	}
}

void ATopdownCameraPawn::BeginPlay()
{
	Super::BeginPlay();
}