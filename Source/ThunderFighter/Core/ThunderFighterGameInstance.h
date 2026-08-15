// ThunderFighter - 雷霆战机 GameInstance
// 跨关卡全局游戏状态管理

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Core/UpgradeTypes.h"
#include "ThunderFighterGameInstance.generated.h"

class UThunderFighterSaveGame;

/**
 * ThunderFighter 的 GameInstance。
 * 存储跨关卡转换的持久化数据：最高分、设置等。
 */
UCLASS()
class THUNDERFIGHTER_API UThunderFighterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	/** 获取有史以来的最高分 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Scoring")
	int32 GetHighScore() const { return HighScore; }

	/** 如果新分数更高则更新最高分 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Scoring")
	bool TryUpdateHighScore(int32 NewScore);

	/** 玩家拥有的炸弹数量 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Gameplay")
	int32 GetBombCount() const { return BombCount; }

	/** 设置炸弹数量 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Gameplay")
	void SetBombCount(int32 Count) { BombCount = Count; }

	/** 玩家剩余生命数 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Gameplay")
	int32 GetLives() const { return Lives; }

	/** 设置玩家生命数 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Gameplay")
	void SetLives(int32 InLives) { Lives = InLives; }

	/** 当前武器等级（死亡后保留） */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Gameplay")
	int32 GetWeaponLevel() const { return WeaponLevel; }

	/** 设置武器等级 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Gameplay")
	void SetWeaponLevel(int32 Level) { WeaponLevel = FMath::Clamp(Level, 1, MaxWeaponLevel); }

	/** 重置所有游戏状态以开始新游戏 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Gameplay")
	void ResetGameState();

	// ---- 局外养成（金币 + 永久升级） ----

	/** 从磁盘加载存档 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Progression")
	void LoadSaveData();

	/** 保存到磁盘 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Progression")
	void SaveData();

	/** 获取金币 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Progression")
	int32 GetGold() const { return Gold; }

	/** 添加金币 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Progression")
	void AddGold(int32 Amount);

	/** 花费金币（成功返回 true） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Progression")
	bool SpendGold(int32 Amount);

	/** 获取某个永久升级的等级 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Progression")
	int32 GetPermanentUpgradeLevel(EPermanentUpgradeType Type) const;

	/** 购买并提升一个永久升级（成功返回 true） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Progression")
	bool UpgradePermanent(EPermanentUpgradeType Type);

	/** 获取某个永久升级的下一级价格 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Progression")
	int32 GetUpgradeCost(EPermanentUpgradeType Type) const;

protected:
	/** 最高分 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Scoring")
	int32 HighScore = 0;

	/** 当前炸弹数量 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Gameplay")
	int32 BombCount = 3;

	/** 玩家生命数 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Gameplay")
	int32 Lives = 3;

	/** 当前武器等级（1-5） */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Gameplay")
	int32 WeaponLevel = 1;

	/** 最大武器等级 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Gameplay")
	int32 MaxWeaponLevel = 5;

	// ---- 局外养成 ----

	/** 金币数量 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Progression")
	int32 Gold = 0;

	/** 各永久升级等级 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Progression")
	TMap<EPermanentUpgradeType, int32> PermanentUpgradeLevels;

	/** 永久升级基础价格 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Progression")
	int32 BaseUpgradeCost = 50;

	/** 每级价格增长倍率 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Progression")
	float CostGrowthPerLevel = 1.5f;

	/** 存档引用 */
	UPROPERTY()
	TObjectPtr<UThunderFighterSaveGame> SaveGameData;
};
