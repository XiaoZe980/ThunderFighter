// ThunderFighter - 雷霆战机 UpgradeSelectWidget 实现

#include "UpgradeSelectWidget.h"
#include "Components/TextBlock.h"

void UUpgradeSelectWidget::ShowOptions(const TArray<FUpgradeDefinition>& Options)
{
	CurrentOptions = Options;

	// 更新三张卡的名称文本（如有绑定）
	TObjectPtr<UTextBlock> NameTexts[] = { UpgradeName1, UpgradeName2, UpgradeName3 };

	for (int32 i = 0; i < 3; i++)
	{
		if (NameTexts[i] && Options.IsValidIndex(i))
		{
			NameTexts[i]->SetText(FText::FromString(Options[i].DisplayName));
		}
	}
}

void UUpgradeSelectWidget::SelectOption(int32 Index)
{
	if (CurrentOptions.IsValidIndex(Index))
	{
		OnUpgradeSelected.Broadcast(Index);
	}
}
