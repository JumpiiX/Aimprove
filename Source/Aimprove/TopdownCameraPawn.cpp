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
                InventoryWidgetInstance->AddToViewport(5); // Higher ZOrder ensures it appears above MainHUD
                UE_LOG(LogTemp, Warning, TEXT("InventoryWidget added to viewport."));
            }
        }
    }
}

// TopdownCameraPawn.cpp - Update HandleBlockPlacement
// TopdownCameraPawn.cpp - Update HandleBlockPlacement
void ATopdownCameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void ATopdownCameraPawn::UpdatePreviewLocation(const FVector& Location, UClass* InBlockClass)
{
   if (!PreviewBlock && InBlockClass)
   {
       FActorSpawnParameters SpawnParams;
       SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
       PreviewBlock = GetWorld()->SpawnActor<AActor>(InBlockClass, Location, FRotator::ZeroRotator, SpawnParams);
       
       if (PreviewBlock)
       {
           TArray<UStaticMeshComponent*> MeshComponents;
           PreviewBlock->GetComponents<UStaticMeshComponent>(MeshComponents);
           
           for (UStaticMeshComponent* MeshComp : MeshComponents)
           {
               MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
               
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
  
   if (PreviewBlock)
   {
       FVector SnappedLocation = FVector(
           FMath::RoundToFloat(Location.X / GridSize) * GridSize,
           FMath::RoundToFloat(Location.Y / GridSize) * GridSize - GridSize,
           Location.Z
       );
       PreviewBlock->SetActorLocation(SnappedLocation);
   }
}

bool ATopdownCameraPawn::HandleBlockPlacement(const FVector& WorldPosition, const FVector& WorldDirection, UClass* InBlockClass)
{
   FVector TraceEnd = WorldPosition + (WorldDirection * PlacementTraceDistance);
   
   FHitResult HitResult;
   FCollisionQueryParams QueryParams;
   QueryParams.AddIgnoredActor(this);
   QueryParams.bTraceComplex = true;

   bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, TraceEnd, ECC_Visibility, QueryParams);
   
   if (bHit && InBlockClass)
   {
       FVector SpawnLocation = FVector(
           FMath::RoundToFloat(HitResult.ImpactPoint.X / GridSize) * GridSize,
           FMath::RoundToFloat(HitResult.ImpactPoint.Y / GridSize) * GridSize - GridSize,
           HitResult.ImpactPoint.Z
       );
       
       FRotator SpawnRotation = FRotator(0.0f, PreviewRotation, 0.0f);
       FActorSpawnParameters SpawnParams;
       SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

       AActor* SpawnedBlock = GetWorld()->SpawnActor<AActor>(InBlockClass, SpawnLocation, SpawnRotation, SpawnParams);
       
       if (SpawnedBlock)
       {
           DestroyPreviewBlock();
           return true;
       }
   }

   return false;
}
void ATopdownCameraPawn::DestroyPreviewBlock()
{
    if (PreviewBlock)
    {
        PreviewBlock->Destroy();
        PreviewBlock = nullptr;
    }
}