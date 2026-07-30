// ThunderFighter - 雷霆战机 GameMode
// 管理游戏规则、计分、波次控制和游戏状态转换

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThunderFighterGameMode.generated.h"

class AEnemySpawner;

/**
 * ThunderFighter 的 GameMode。
 * 控制波次生成、计分和游戏结束条件。
 */
UCLASS()
class THUNDERFIGHTER_API AThunderFighterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AThunderFighterGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** 为玩家添加分数 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Scoring")
	void AddScore(int32 Points);

	/** 获取当前分数 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Scoring")
	int32 GetScore() const { return CurrentScore; }

	/** 玩家被击败——触发游戏结束 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|GameState")
	void OnPlayerDefeated();

	/** 重新开始当前关卡 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|GameState")
	void RestartGame();

	/** 返回主菜单 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|GameState")
	void ReturnToMainMenu();

protected:
	/** 本局累计分数 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Scoring")
	int32 CurrentScore = 0;

	/** 游戏是否已结束？ */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|GameState")
	bool bGameOver = false;

	/** 游戏结束动作触发前的等待时间（秒） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|GameState")
	float GameOverDelay = 3.0f;

	/** 游戏结束延迟计时器句柄 */
	float GameOverTimer = 0.0f;

	/** 关卡中敌人生成器的引用 */
	UPROPERTY()
	TWeakObjectPtr<AEnemySpawner> EnemySpawnerRef;

	/** 主菜单关卡名称 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Levels")
	FName MainMenuLevelName = TEXT("MainMenu");
};
