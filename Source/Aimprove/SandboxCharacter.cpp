#include "SandboxCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ASandboxCharacter::ASandboxCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
    
	// Load the material in constructor
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMaterialFinder(TEXT("/Game/Weapons/M_ImpactDecal"));
	if (DecalMaterialFinder.Succeeded())
	{
		ImpactDecalMaterial = DecalMaterialFinder.Object;
	}
}
void ASandboxCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASandboxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASandboxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
	// Bind Shoot function to P key
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ASandboxCharacter::Shoot);
}

void ASandboxCharacter::Shoot()
{
	// Get player camera viewpoint
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// Calculate shot direction
	FVector ShotDirection = CameraRotation.Vector();
	FVector TraceEnd = CameraLocation + (ShotDirection * 10000.0f);

	// Setup trace parameters
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// Perform line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	// If we hit something, spawn a decal
	if (bHit && ImpactDecalMaterial)
	{
		FRotator DecalRotation = HitResult.Normal.Rotation();
        
		// Spawn decal at hit location
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			ImpactDecalMaterial,
			FVector(DecalSize, DecalSize, DecalSize),
			HitResult.Location,
			DecalRotation,
			DecalLifespan
		);
	}
}