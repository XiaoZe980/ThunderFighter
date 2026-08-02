// ThunderFighter - 雷霆战机 BossHealthBarWidget
// Boss 专属血条 Widget，显示 Boss 血量百分比与名称

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHealthBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * Boss 血条 Widget。蓝图 WBP_BossHealthBar 继承此类，
 * 并在 Widget 布局中放置同名控件（HealthBar / BossNameText）完成绑定。
 */
UCLASS()
class THUNDERFIGHTER_API UBossHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * 更新 Boss 血条显示。
	 * @param HealthPercent 血量百分比（0.0-1.0）
	 * @param BossName Boss 显示名称
	 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void SetBossInfo(float HealthPercent, const FString& BossName);

protected:
	/** 血量进度条 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	/** Boss 名称文本 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BossNameText;
};
