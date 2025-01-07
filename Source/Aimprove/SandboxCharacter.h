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

private:
	void Shoot();
	void EarnCoins(int32 Amount);

	/** Number of coins earned by the player */
	int32 Coins;

	/** Material used for impact decals */
	UPROPERTY()
	class UMaterialInterface* ImpactDecalMaterial;

	/** Size and lifespan for impact decals */
	float DecalSize = 10.0f;
	float DecalLifespan = 5.0f;
};
