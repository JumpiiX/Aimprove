#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMainHUD.generated.h"

UCLASS()
class AIMPROVE_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateCoinCount(int32 Coins);
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CoinText;
protected:
	
};
