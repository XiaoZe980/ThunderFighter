// ThunderFighter - 雷霆战机 UpgradeShopWidget
// 局外养成商店 UI：金币购买永久升级

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeShopWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 升级商店 Widget。蓝图 WBP_UpgradeShop 继承此类，
 * 放置同名控件完成绑定。
 * C++ 处理购买逻辑并自动刷新金币/等级/价格显示。
 */
UCLASS()
class THUNDERFIGHTER_API UUpgradeShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	/** 刷新金币、各项升级的等级与价格显示 */
	void RefreshDisplay();

	// ---- 按钮 ----
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UButton> DamageButton;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UButton> HealthButton;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UButton> FireRateButton;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UButton> SpeedButton;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UButton> BackButton;

	// ---- 文本 ----
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> GoldText;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> DamageInfoText;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> HealthInfoText;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> FireRateInfoText;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> SpeedInfoText;

	// ---- 点击回调 ----
	UFUNCTION()
	void OnDamageClicked();
	UFUNCTION()
	void OnHealthClicked();
	UFUNCTION()
	void OnFireRateClicked();
	UFUNCTION()
	void OnSpeedClicked();
	UFUNCTION()
	void OnBackClicked();
};
