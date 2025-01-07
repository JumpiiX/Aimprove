#include "SandboxCharacter.h"
#include "CustomPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UMainHUD.h"

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

        // Add debug logs for trace
        UE_LOG(LogTemp, Warning, TEXT("Shooting from: %s towards: %s"), *CameraLocation.ToString(), *TraceEnd.ToString());

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        QueryParams.bTraceComplex = true; // Add this for more precise collision

        // Try ECC_PhysicsBody instead
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            CameraLocation,
            TraceEnd,
            ECC_PhysicsBody,
            QueryParams
        );

        if (bHit)
        {
            if (HitResult.GetActor())
            {
                UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s at location: %s"), 
                    *HitResult.GetActor()->GetName(), 
                    *HitResult.Location.ToString());
                UE_LOG(LogTemp, Warning, TEXT("Hit component: %s"), 
                    *HitResult.Component->GetName());
            }
        if (ImpactDecalMaterial)
        {
            FRotator DecalRotation = HitResult.Normal.Rotation();
            UGameplayStatics::SpawnDecalAtLocation(
                GetWorld(),
                ImpactDecalMaterial,
                FVector(DecalSize, DecalSize, DecalSize),
                HitResult.Location,
                DecalRotation,
                DecalLifespan
            );
        }

        UClass* EnemyBlueprintClass = LoadObject<UClass>(nullptr, TEXT("/Game/Blueprints/CBP_Enemy.CBP_Enemy_C"));
        if (EnemyBlueprintClass && HitResult.GetActor() && HitResult.GetActor()->IsA(EnemyBlueprintClass))
        {
            UE_LOG(LogTemp, Warning, TEXT("Enemy hit detected! Awarding coins."));
            EarnCoins(100);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Shoot: No hit detected."));
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
