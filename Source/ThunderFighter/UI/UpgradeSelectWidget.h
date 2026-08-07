// ThunderFighter - 雷霆战机 UpgradeSelectWidget
// 升级三选一强化卡界面

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/UpgradeTypes.h"
#include "UpgradeSelectWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeSelected, int32, SelectedIndex);

/**
 * 三选一强化卡界面。蓝图 WBP_UpgradeSelect 继承此类。
 * GameMode 升级暂停后调用 ShowOptions 显示 3 张强化卡，
 * 玩家点击后通过 OnUpgradeSelected 回调通知 GameMode 应用强化。
 */
UCLASS()
class THUNDERFIGHTER_API UUpgradeSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 显示 3 个强化选项 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Upgrade")
	void ShowOptions(const TArray<FUpgradeDefinition>& Options);

	/** 玩家选择回调（GameMode 绑定） */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Upgrade")
	FOnUpgradeSelected OnUpgradeSelected;

	/** 玩家点击强化卡（蓝图按钮事件绑定到这三个函数） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Upgrade")
	void SelectOption(int32 Index);

protected:
	/** 根据索引更新卡片显示（蓝图实现或 C++ 绑定控件） */
	void UpdateCard(int32 Index, const FUpgradeDefinition& Upgrade);

	// 三张卡的控件（BindWidget 可选，蓝图实现也可直接读成员）
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Upgrade")
	TArray<FUpgradeDefinition> CurrentOptions;

	// ---- 可选绑定控件（蓝图用同名控件自动绑定） ----
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> UpgradeName1;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> UpgradeName2;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> UpgradeName3;
};
