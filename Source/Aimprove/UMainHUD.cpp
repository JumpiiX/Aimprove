#include "UMainHUD.h"
#include "Components/TextBlock.h"

void UMainHUD::UpdateCoinCount(int32 Coins)
{
	if (CoinText)
	{
		CoinText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Coins)));
		UE_LOG(LogTemp, Warning, TEXT("UMainHUD: CoinText updated to %d."), Coins);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainHUD: CoinText is null. Check widget binding."));
	}
}