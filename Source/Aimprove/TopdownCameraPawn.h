// TopdownCameraPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "InventoryWidget.h"
#include "TopdownCameraPawn.generated.h"

// Forward declare the types we need
class UDragDropOperation;
struct FGeometry;

UCLASS()
class AIMPROVE_API ATopdownCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ATopdownCameraPawn();
	void UpdatePreviewLocation(const FVector& Location);
	void DestroyPreviewBlock();
	// Change to simpler function signature that doesn't expose Slate types in header
	UFUNCTION()
	bool HandleBlockPlacement(const FVector& WorldPosition, const FVector& WorldDirection);
	UPROPERTY()
	UInventoryWidget* InventoryWidgetInstance;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* TopdownCameraComponent;
	UPROPERTY()
	AActor* PreviewBlock;

	UPROPERTY(EditAnywhere, Category = "Building")
	float GridSize = 100.0f;
	

	// Add the placement trace distance property
	UPROPERTY(EditAnywhere, Category = "Building")
	float PlacementTraceDistance = 10000.0f;

	void CreateInventoryWidget();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE UCameraComponent* GetTopdownCamera() const { return TopdownCameraComponent; }
};