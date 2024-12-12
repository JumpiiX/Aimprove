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
    QueryParams.bTraceComplex = true;  // Für genauere Kollisionserkennung

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, TraceEnd, ECC_Visibility, QueryParams);
    
    if (bHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit at: %s"), *HitResult.Location.ToString());
        
        UClass* BlockClass = LoadClass<AActor>(nullptr, TEXT("/Game/LevelPrototyping/LevelBlock_Traversable.LevelBlock_Traversable_C"));
        if (!BlockClass)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load block class!"));
            return false;
        }

        // Snappen der Position zum Grid
        FVector SpawnLocation = FVector(
            FMath::RoundToFloat(HitResult.ImpactPoint.X / GridSize) * GridSize,
            FMath::RoundToFloat(HitResult.ImpactPoint.Y / GridSize) * GridSize,
            HitResult.ImpactPoint.Z
        );

        UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn at: %s"), *SpawnLocation.ToString());
        
        FRotator SpawnRotation = FRotator::ZeroRotator;
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedBlock = GetWorld()->SpawnActor<AActor>(BlockClass, SpawnLocation, SpawnRotation, SpawnParams);
        
        if (SpawnedBlock)
        {
            UE_LOG(LogTemp, Warning, TEXT("Successfully spawned block: %s"), *SpawnedBlock->GetName());
            DestroyPreviewBlock();  // Preview zerstören nach erfolgreicher Platzierung
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

void ATopdownCameraPawn::UpdatePreviewLocation(const FVector& Location)
{
    // Erstelle Preview-Block falls noch nicht vorhanden
    if (!PreviewBlock)
    {
        UClass* BlockClass = LoadClass<AActor>(nullptr, TEXT("/Game/LevelPrototyping/LevelBlock_Traversable.LevelBlock_Traversable_C"));
        if (BlockClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            PreviewBlock = GetWorld()->SpawnActor<AActor>(BlockClass, Location, FRotator::ZeroRotator, SpawnParams);
            
            if (PreviewBlock)
            {
                TArray<UStaticMeshComponent*> MeshComponents;
                PreviewBlock->GetComponents<UStaticMeshComponent>(MeshComponents);
                
                for (UStaticMeshComponent* MeshComp : MeshComponents)
                {
                    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                    
                    // Setze alle Materialien auf halbtransparent
                    for (int32 i = 0; i < MeshComp->GetNumMaterials(); i++)
                    {
                        UMaterialInterface* Material = MeshComp->GetMaterial(i);
                        if (Material)
                        {
                            UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material, this);
                            DynMaterial->SetScalarParameterValue(TEXT("Opacity"), 0.5f);
                            MeshComp->SetMaterial(i, DynMaterial);
                        }
                    }
                }
            }
        }
    }
    
    // Update Position
    if (PreviewBlock)
    {
        FVector SnappedLocation = FVector(
            FMath::RoundToFloat(Location.X / GridSize) * GridSize,
            FMath::RoundToFloat(Location.Y / GridSize) * GridSize,
            Location.Z
        );
        PreviewBlock->SetActorLocation(SnappedLocation);
    }
}

void ATopdownCameraPawn::DestroyPreviewBlock()
{
    if (PreviewBlock)
    {
        PreviewBlock->Destroy();
        PreviewBlock = nullptr;
    }
}