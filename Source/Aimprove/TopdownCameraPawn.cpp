// TopdownCameraPawn.cpp
#include "TopdownCameraPawn.h"
#include "InventoryWidget.h"

ATopdownCameraPawn::ATopdownCameraPawn()
{
    // Enable tick
    PrimaryActorTick.bCanEverTick = true;

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

    UE_LOG(LogTemp, Warning, TEXT("TopdownCameraPawn BeginPlay called"));

    // Add a small delay before creating the widget
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATopdownCameraPawn::CreateInventoryWidget, 0.1f, false);
}

// Add this new function
void ATopdownCameraPawn::CreateInventoryWidget()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found PlayerController"));
        
        // Load the widget class
        UClass* WidgetClass = LoadClass<UInventoryWidget>(nullptr, TEXT("/Game/UI/WBP_InventoryWidget.WBP_InventoryWidget_C"));
        if (WidgetClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("Widget Class loaded successfully"));
            
            // Create the widget
            InventoryWidgetInstance = CreateWidget<UInventoryWidget>(PC, WidgetClass);
            if (InventoryWidgetInstance)
            {
                UE_LOG(LogTemp, Warning, TEXT("Widget Instance created successfully"));
                InventoryWidgetInstance->AddToViewport();
                UE_LOG(LogTemp, Warning, TEXT("Widget added to viewport"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create widget instance"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load widget class"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No PlayerController found"));
    }
}

void ATopdownCameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}