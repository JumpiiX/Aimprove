#include "SandboxCharacter.h"
#include "CustomPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UMainHUD.h"
#include "Components/DecalComponent.h"

ASandboxCharacter::ASandboxCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Find and set up the impact decal material
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMaterialFinder(TEXT("/Game/Weapons/M_ImpactDecal"));
    if (DecalMaterialFinder.Succeeded())
    {
        ImpactDecalMaterial = DecalMaterialFinder.Object;
    }

    Coins = 0; // Initialize coin count
}

void ASandboxCharacter::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("ASandboxCharacter::BeginPlay called."));
}

void ASandboxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Add logic if required to be updated every frame
}

void ASandboxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind the "Shoot" action to the Shoot function
    PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ASandboxCharacter::Shoot);
}

void ASandboxCharacter::Shoot()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector ShotDirection = CameraRotation.Vector();
    FVector TraceEnd = CameraLocation + (ShotDirection * 10000.0f);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.bTraceComplex = true;

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        CameraLocation,
        TraceEnd,
        ECC_PhysicsBody,
        QueryParams
    );

    if (bHit)
    {
        // Check for enemy hit first
        UClass* EnemyBlueprintClass = LoadObject<UClass>(nullptr, TEXT("/Game/Blueprints/CBP_Enemy.CBP_Enemy_C"));
        if (EnemyBlueprintClass && HitResult.GetActor() && HitResult.GetActor()->IsA(EnemyBlueprintClass))
        {
            UE_LOG(LogTemp, Warning, TEXT("Enemy hit detected! Awarding coins."));
            EarnCoins(100);
            
            if (ACustomPlayerController* CustomPC = Cast<ACustomPlayerController>(GetController()))
            {
                CustomPC->RestartRound();
            }
        }

        // Spawn decal regardless of what was hit
        if (ImpactDecalMaterial)
        {
            FRotator DecalRotation = HitResult.Normal.Rotation();
            UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
                GetWorld(),
                ImpactDecalMaterial,
                FVector(DecalSize, DecalSize, DecalSize),
                HitResult.Location,
                DecalRotation,
                DecalLifespan
            );

            if (Decal)
            {
                // Force decal to be visible at any distance
                Decal->FadeScreenSize = 0.0f;
                Decal->SetFadeScreenSize(0.0f);
            }
        
        }
    }
}




void ASandboxCharacter::EarnCoins(int32 Amount)
{
    Coins += Amount; // Update the coin count
    UE_LOG(LogTemp, Warning, TEXT("EarnCoins called. Current coin count: %d"), Coins);

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        // Access the custom player controller and update the HUD
        ACustomPlayerController* CustomPC = Cast<ACustomPlayerController>(PC);
        if (CustomPC)
        {
            if (CustomPC->MainHUD)
            {
                UE_LOG(LogTemp, Warning, TEXT("EarnCoins: Updating MainHUD with new coin count: %d"), Coins);
                CustomPC->MainHUD->UpdateCoinCount(Coins);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("EarnCoins: MainHUD is null. Ensure it is initialized in CustomPlayerController."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("EarnCoins: Could not cast to ACustomPlayerController."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EarnCoins: No PlayerController found."));
    }
}
