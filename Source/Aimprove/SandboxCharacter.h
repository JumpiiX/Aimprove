#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SandboxCharacter.generated.h"

/**
 * Sandbox character that interacts with the environment.
 */
UCLASS()
class AIMPROVE_API ASandboxCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASandboxCharacter();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	class UMaterialInterface* ImpactDecalMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float DecalFadeScreenSize = 0.0f; // Set to 0 for maximum visibility distance

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float DecalSize = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float DecalLifespan = 999999.0f;
	void Shoot();
	void EarnCoins(int32 Amount);

	/** Number of coins earned by the player */
	int32 Coins;


};
