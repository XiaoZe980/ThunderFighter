// ThunderFighter - 雷霆战机 HUD
// 游戏内抬头显示，展示分数、生命值、武器等级、炸弹数量

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ThunderFighterHUD.generated.h"

class UUserWidget;
class ABossEnemy;

/**
 * ThunderFighter 的 HUD 类。
 * 管理游戏内 UI 的 UMG 控件：分数、生命值条、武器等级、炸弹、游戏结束画面。
 */
UCLASS()
class THUNDERFIGHTER_API AThunderFighterHUD : public AHUD
{
	GENERATED_BODY()

public:
	AThunderFighterHUD();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

public:
	/** 显示游戏玩法 HUD */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void ShowGameplayHUD();

	/** 隐藏游戏玩法 HUD */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void HideGameplayHUD();

	/** 显示游戏结束画面 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void ShowGameOverScreen(int32 FinalScore, int32 HighScore);

	/** 显示暂停菜单 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void ShowPauseMenu();

	/** 隐藏暂停菜单 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void HidePauseMenu();

	/** 显示 Boss 血条 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void ShowBossHealthBar();

	/** 隐藏 Boss 血条 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void HideBossHealthBar();

	// ---- 蓝图控件类型 ----

	/** 游戏玩法 HUD 控件类型（分数、生命值等） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|UI")
	TSubclassOf<UUserWidget> GameplayHUDClass;

	/** 游戏结束控件类型 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	/** 暂停菜单控件类型 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	/** Boss 血条控件类型 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|UI")
	TSubclassOf<UUserWidget> BossHealthBarClass;

protected:
	/** 当前活跃的游戏玩法 HUD 控件实例 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|UI")
	TObjectPtr<UUserWidget> GameplayHUDWidget;

	/** 当前活跃的游戏结束控件实例 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|UI")
	TObjectPtr<UUserWidget> GameOverWidget;

	/** 当前活跃的暂停菜单控件实例 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|UI")
	TObjectPtr<UUserWidget> PauseMenuWidget;

	/** 当前活跃的 Boss 血条控件实例 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|UI")
	TObjectPtr<UUserWidget> BossHealthBarWidget;
};
