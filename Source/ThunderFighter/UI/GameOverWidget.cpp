// ThunderFighter - 雷霆战机 GameOverWidget 实现

#include "GameOverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 绑定按钮点击事件
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnRestartClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnMainMenuClicked);
	}

	// 默认隐藏新纪录提示
	if (NewRecordText)
	{
		NewRecordText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGameOverWidget::SetScoreInfo(int32 FinalScore, int32 HighScore)
{
	if (FinalScoreText)
	{
		FinalScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), FinalScore)));
	}

	if (HighScoreText)
	{
		HighScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), HighScore)));
	}
}

void UGameOverWidget::SetIsNewRecord(bool bNewRecord)
{
	if (NewRecordText)
	{
		NewRecordText->SetVisibility(bNewRecord ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UGameOverWidget::OnRestartClicked()
{
	// 重新打开当前关卡
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void UGameOverWidget::OnMainMenuClicked()
{
	// 打开主菜单关卡（默认名称 MainMenu，可在 GameMode 配置）
	UGameplayStatics::OpenLevel(this, TEXT("MainMenu"));
}
