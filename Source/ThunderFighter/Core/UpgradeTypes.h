// ThunderFighter - 雷霆战机 UpgradeTypes
// 强化卡系统的核心类型定义：强化效果枚举 + 强化卡数据结构

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UpgradeTypes.generated.h"

class UTexture2D;

/**
 * 强化卡可产生的效果类型。
 * 每种效果对应 WeaponComponent 或 PlayerPawn 上的一个修改。
 */
UENUM(BlueprintType)
enum class EUpgradeEffect : uint8
{
	// ---- 武器类 ----
	BonusBullets		UMETA(DisplayName = "多重射击（+弹道）"),
	DamageMultiplier	UMETA(DisplayName = "强化弹头（+伤害）"),
	FireRateMultiplier	UMETA(DisplayName = "过载射击（+射速）"),
	Piercing			UMETA(DisplayName = "贯穿弹（穿透）"),
	Homing				UMETA(DisplayName = "追踪导弹"),
	Bounce				UMETA(DisplayName = "弹跳弹"),
	SideWings			UMETA(DisplayName = "侧翼僚机"),

	// ---- 生存类 ----
	MaxHealth			UMETA(DisplayName = "装甲强化（+血量）"),
	Shield				UMETA(DisplayName = "能量护盾"),
	Regeneration		UMETA(DisplayName = "再生（击杀回血）"),
	InvincibilityTime	UMETA(DisplayName = "不屈（+无敌时间）"),

	// ---- 移动类 ----
	MoveSpeed			UMETA(DisplayName = "矢量引擎（+移速）"),

	// ---- 特殊类 ----
	BombCount			UMETA(DisplayName = "弹药补给（+炸弹）"),
	ExpGainMultiplier	UMETA(DisplayName = "能量收集（+经验）"),
	MagnetRadius		UMETA(DisplayName = "磁力（+吸附范围）")
};

/**
 * 强化卡稀有度。
 */
UENUM(BlueprintType)
enum class EUpgradeRarity : uint8
{
	Common		UMETA(DisplayName = "普通"),
	Rare		UMETA(DisplayName = "稀有"),
	Epic		UMETA(DisplayName = "史诗"),
	Legendary	UMETA(DisplayName = "传说")
};

/**
 * 单个强化卡的定义。
 * 通过 DataTable（DT_Upgrades）配置，蓝图可直接编辑。
 */
USTRUCT(BlueprintType)
struct FUpgradeDefinition : public FTableRowBase
{
	GENERATED_BODY()

	/** 强化卡唯一 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	FName UpgradeID;

	/** 显示名称 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	FString DisplayName;

	/** 描述文本 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	FString Description;

	/** 图标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	TObjectPtr<UTexture2D> Icon;

	/** 效果类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	EUpgradeEffect Effect = EUpgradeEffect::DamageMultiplier;

	/** 效果数值（伤害倍率 0.5=+50%，弹道数 1=+1 等） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	float EffectValue = 1.0f;

	/** 该强化最大可叠加次数（1=不可重复，>1=可叠加） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	int32 MaxStacks = 1;

	/** 稀有度（影响颜色和抽取权重） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	EUpgradeRarity Rarity = EUpgradeRarity::Common;

	/** 抽取权重（越大越常出现） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	float Weight = 1.0f;
};
