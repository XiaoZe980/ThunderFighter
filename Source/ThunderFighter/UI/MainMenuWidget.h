// ThunderFighter - 雷霆战机 MainMenuWidget
// 主菜单 UI：开始游戏 / 升级商店 / 退出

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 主菜单 Widget。蓝图 WBP_MainMenu 继承此类，
 * 放置同名控件（StartButton / ShopButton / QuitButton / GoldText）完成绑定。
 * C++ 处理按钮点击逻辑：开始游戏、进入商店、退出。
 */
UCLASS()
class THUNDERFIGHTER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	/** 开始游戏按钮 */
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UButton> StartButton;

	/** 升级商店按钮 */
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UButton> ShopButton;

	/** 退出按钮 */
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UButton> QuitButton;

	/** 金币显示文本 */
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> GoldText;

	/** 开始游戏 */
	UFUNCTION()
	void OnStartClicked();

	/** 打开升级商店 */
	UFUNCTION()
	void OnShopClicked();

	/** 退出游戏 */
	UFUNCTION()
	void OnQuitClicked();
};
