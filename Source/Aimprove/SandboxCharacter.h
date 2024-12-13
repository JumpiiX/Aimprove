#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SandboxCharacter.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AIMPROVE_API ASandboxCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASandboxCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Shooting function
	UFUNCTION(BlueprintCallable, Category = "Shooting")
	void Shoot();

	// Decal material reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	UMaterialInterface* ImpactDecalMaterial;

	// Decal size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float DecalSize = 10.0f;

	// Decal lifetime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float DecalLifespan = 5.0f;

protected:
	virtual void BeginPlay() override;
};