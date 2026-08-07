// ThunderFighter - 雷霆战机 PlayerLevelComponent
// 玩家经验/等级管理：击杀收集经验，攒满升级

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerLevelComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLevelUp);

/**
 * 附加到玩家 Pawn，管理经验值、等级和升级事件。
 * 击杀敌人获得经验球（EnergyCrystal）时调用 AddExperience。
 * 攒满经验触发 OnLevelUp（GameMode 监听，暂停并弹出强化选择）。
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UPlayerLevelComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerLevelComponent();

	/** 添加经验值，攒满自动升级 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Level")
	void AddExperience(float Amount);

	/** 当前等级 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Level")
	int32 GetLevel() const { return Level; }

	/** 当前经验值（本等级内） */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Level")
	float GetExperience() const { return Experience; }

	/** 升级所需经验值 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Level")
	float GetExperienceToNext() const { return ExperienceToNext; }

	/** 本等级进度（0.0-1.0），供经验条显示 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Level")
	float GetLevelProgress() const
	{
		return ExperienceToNext > 0.0f ? FMath::Clamp(Experience / ExperienceToNext, 0.0f, 1.0f) : 0.0f;
	}

	/** 升级时触发（GameMode 监听后暂停+弹强化选择） */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Level")
	FOnPlayerLevelUp OnLevelUp;

	/** 设置经验获取倍率（能量收集强化） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Level")
	void SetExpMultiplier(float Multiplier) { ExpMultiplier = FMath::Max(0.01f, Multiplier); }

	/** 获取经验获取倍率 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Level")
	float GetExpMultiplier() const { return ExpMultiplier; }

protected:
	/** 1 级升 2 级所需经验 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Level")
	float BaseXPToNext = 10.0f;

	/** 经验需求增长倍率（每级 × 该值） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Level")
	float XPGrowthFactor = 1.5f;

	/** 当前等级（从 1 开始） */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Level")
	int32 Level = 1;

	/** 当前等级内已积累经验 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Level")
	float Experience = 0.0f;

	/** 升级所需经验（当前等级） */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Level")
	float ExperienceToNext = 10.0f;

	/** 经验获取倍率（强化加成） */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Level")
	float ExpMultiplier = 1.0f;

private:
	/** 检查并处理升级（可能连续升多级） */
	void CheckLevelUp();
};
