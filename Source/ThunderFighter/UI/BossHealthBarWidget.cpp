// ThunderFighter - 雷霆战机 BossHealthBarWidget 实现

#include "BossHealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBossHealthBarWidget::SetBossInfo(float HealthPercent, const FString& BossName)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(FMath::Clamp(HealthPercent, 0.0f, 1.0f));
	}

	if (BossNameText)
	{
		BossNameText->SetText(FText::FromString(BossName));
	}
}
