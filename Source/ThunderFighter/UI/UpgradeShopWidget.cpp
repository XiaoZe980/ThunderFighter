// ThunderFighter - 雷霆战机 UpgradeShopWidget 实现

#include "UpgradeShopWidget.h"
#include "Core/ThunderFighterGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UUpgradeShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (DamageButton) DamageButton->OnClicked.AddDynamic(this, &UUpgradeShopWidget::OnDamageClicked);
	if (HealthButton) HealthButton->OnClicked.AddDynamic(this, &UUpgradeShopWidget::OnHealthClicked);
	if (FireRateButton) FireRateButton->OnClicked.AddDynamic(this, &UUpgradeShopWidget::OnFireRateClicked);
	if (SpeedButton) SpeedButton->OnClicked.AddDynamic(this, &UUpgradeShopWidget::OnSpeedClicked);
	if (BackButton) BackButton->OnClicked.AddDynamic(this, &UUpgradeShopWidget::OnBackClicked);

	RefreshDisplay();
}

void UUpgradeShopWidget::RefreshDisplay()
{
	UThunderFighterGameInstance* GI = Cast<UThunderFighterGameInstance>(GetGameInstance());
	if (!GI) return;

	if (GoldText)
	{
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GI->GetGold())));
	}

	// 每个升级项：名称 + 等级 + 价格
	auto UpdateInfo = [&](UTextBlock* Text, EPermanentUpgradeType Type, const TCHAR* DisplayName)
	{
		if (Text)
		{
			int32 Level = GI->GetPermanentUpgradeLevel(Type);
			int32 Cost = GI->GetUpgradeCost(Type);
			Text->SetText(FText::FromString(
				FString::Printf(TEXT("%s  Lv.%d  价格: %d"), DisplayName, Level, Cost)));
		}
	};

	UpdateInfo(DamageInfoText, EPermanentUpgradeType::Damage, TEXT("伤害强化"));
	UpdateInfo(HealthInfoText, EPermanentUpgradeType::Health, TEXT("装甲强化"));
	UpdateInfo(FireRateInfoText, EPermanentUpgradeType::FireRate, TEXT("射速强化"));
	UpdateInfo(SpeedInfoText, EPermanentUpgradeType::Speed, TEXT("机动强化"));
}

void UUpgradeShopWidget::OnDamageClicked()
{
	if (UThunderFighterGameInstance* GI = Cast<UThunderFighterGameInstance>(GetGameInstance()))
	{
		GI->UpgradePermanent(EPermanentUpgradeType::Damage);
		RefreshDisplay();
	}
}

void UUpgradeShopWidget::OnHealthClicked()
{
	if (UThunderFighterGameInstance* GI = Cast<UThunderFighterGameInstance>(GetGameInstance()))
	{
		GI->UpgradePermanent(EPermanentUpgradeType::Health);
		RefreshDisplay();
	}
}

void UUpgradeShopWidget::OnFireRateClicked()
{
	if (UThunderFighterGameInstance* GI = Cast<UThunderFighterGameInstance>(GetGameInstance()))
	{
		GI->UpgradePermanent(EPermanentUpgradeType::FireRate);
		RefreshDisplay();
	}
}

void UUpgradeShopWidget::OnSpeedClicked()
{
	if (UThunderFighterGameInstance* GI = Cast<UThunderFighterGameInstance>(GetGameInstance()))
	{
		GI->UpgradePermanent(EPermanentUpgradeType::Speed);
		RefreshDisplay();
	}
}

void UUpgradeShopWidget::OnBackClicked()
{
	UGameplayStatics::OpenLevel(this, TEXT("L_MainMenu"));
}
