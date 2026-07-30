// ThunderFighter - 雷霆战机 GameInstance
// 跨关卡全局游戏状态管理

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ThunderFighterGameInstance.generated.h"

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
};
