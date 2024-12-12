// TopdownCameraPawn.cpp
#include "TopdownCameraPawn.h"
#include "InventoryWidget.h"
#include "InventoryDragDropOperation.h"
#include "Kismet/GameplayStatics.h"

ATopdownCameraPawn::ATopdownCameraPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    TopdownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopdownCamera"));
    if (TopdownCameraComponent)
    {
        TopdownCameraComponent->SetupAttachment(RootComponent);
    }
}

void ATopdownCameraPawn::BeginPlay()
{
    Super::BeginPlay();
    
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATopdownCameraPawn::CreateInventoryWidget, 0.1f, false);
}

void ATopdownCameraPawn::CreateInventoryWidget()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        UClass* WidgetClass = LoadClass<UInventoryWidget>(nullptr, TEXT("/Game/UI/WBP_InventoryWidget.WBP_InventoryWidget_C"));
        if (WidgetClass)
        {
            InventoryWidgetInstance = CreateWidget<UInventoryWidget>(PC, WidgetClass);
            if (InventoryWidgetInstance)
            {
                InventoryWidgetInstance->AddToViewport();
            }
        }
    }
}

// TopdownCameraPawn.cpp - Update HandleBlockPlacement
// TopdownCameraPawn.cpp - Update HandleBlockPlacement
bool ATopdownCameraPawn::HandleBlockPlacement(const FVector& WorldPosition, const FVector& WorldDirection)
{
    UE_LOG(LogTemp, Warning, TEXT("=== Attempting Block Placement ==="));
    UE_LOG(LogTemp, Warning, TEXT("WorldPosition: %s"), *WorldPosition.ToString());
    UE_LOG(LogTemp, Warning, TEXT("WorldDirection: %s"), *WorldDirection.ToString());
    
    FVector TraceEnd = WorldPosition + (WorldDirection * PlacementTraceDistance);
    
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, TraceEnd, ECC_Visibility, QueryParams);
    
    if (bHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit at: %s"), *HitResult.Location.ToString());
        
        // Versuchen wir beide LevelBlock Varianten
        UClass* BlockClass = LoadClass<AActor>(nullptr, TEXT("/Game/LevelPrototyping/LevelBlock_Traversable.LevelBlock_Traversable_C"));
        if (!BlockClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("Trying normal LevelBlock instead..."));
            BlockClass = LoadClass<AActor>(nullptr, TEXT("/Game/LevelPrototyping/LevelBlock.LevelBlock_C"));
            
            if (!BlockClass)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to load both block classes!"));
                return false;
            }
        }

        FVector SpawnLocation = HitResult.Location;
        SpawnLocation.Z += 50.0f;

        UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn at: %s"), *SpawnLocation.ToString());
        
        FRotator SpawnRotation = FRotator::ZeroRotator;
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedBlock = GetWorld()->SpawnActor<AActor>(BlockClass, SpawnLocation, SpawnRotation, SpawnParams);
        
        if (SpawnedBlock)
        {
            UE_LOG(LogTemp, Warning, TEXT("Successfully spawned block: %s"), *SpawnedBlock->GetName());
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn block at location"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Line trace did not hit anything - No surface to place block on"));
    }

    return false;
}

void ATopdownCameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}