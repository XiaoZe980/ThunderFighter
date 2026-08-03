// ThunderFighter - 雷霆战机 GameOverWidget
// 游戏结束结算界面：显示最终分数、最高分、重开/返回按钮

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UTextBlock;
class UButton;

/**
 * 游戏结束结算 Widget。蓝图 WBP_GameOver 继承此类，
 * 放置同名控件（FinalScoreText / HighScoreText / RestartButton / MainMenuButton）完成绑定。
 */
UCLASS()
class THUNDERFIGHTER_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * 设置结算分数信息。
	 * @param FinalScore 本局最终得分
	 * @param HighScore 历史最高分
	 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void SetScoreInfo(int32 FinalScore, int32 HighScore);

	/** 是否刷新了最高分（用于显示 "新纪录!" 提示） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void SetIsNewRecord(bool bNewRecord);

protected:
	virtual void NativeOnInitialized() override;

	/** 最终分数文本 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FinalScoreText;

	/** 最高分文本 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HighScoreText;

	/** 新纪录提示文本（可选，蓝图可不放） */
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> NewRecordText;

	/** 重新开始按钮 */
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UButton> RestartButton;

	/** 返回主菜单按钮 */
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UButton> MainMenuButton;

	/** 点击重新开始 */
	UFUNCTION()
	void OnRestartClicked();

	/** 点击返回主菜单 */
	UFUNCTION()
	void OnMainMenuClicked();
};
