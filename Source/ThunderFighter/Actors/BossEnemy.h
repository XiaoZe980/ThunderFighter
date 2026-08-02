// ThunderFighter - 雷霆战机 BossEnemy
// Boss 敌机：多阶段状态机 + 悬停扫射移动 + 专属弹幕

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "BossEnemy.generated.h"

class AProjectileBase;

/** Boss 阶段的弹幕模式类型 */
UENUM(BlueprintType)
enum class EBossPatternType : uint8
{
	Aimed		UMETA(DisplayName = "瞄准射击"),
	Spread		UMETA(DisplayName = "扇形散射"),
	Circle		UMETA(DisplayName = "环形弹幕")
};

/** 单个 Boss 阶段的配置 */
USTRUCT(BlueprintType)
struct FEnemyPhase
{
	GENERATED_BODY()

	/** 血量门限比例（0.0-1.0），血量降到该比例时进入此阶段 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	float HealthThreshold = 1.0f;

	/** 本阶段使用的弹幕模式 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	EBossPatternType PatternType = EBossPatternType::Aimed;

	/** 射击间隔（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	float FireInterval = 1.5f;

	/** 单次发射的子弹数量（扇形/环形有效） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	int32 BulletCount = 5;

	/** 扇形张开角度（度，仅扇形有效） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	float SpreadAngle = 60.0f;

	/** 子弹速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	float BulletSpeed = 600.0f;

	/** 子弹伤害 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	float BulletDamage = 10.0f;
};

/**
 * Boss 敌机。继承 AEnemyBase 获得生命值/计分/掉落逻辑。
 * 覆盖 ApplyMovement 实现「入场 → 悬停扫射」移动。
 * 监听 OnHealthChanged 按血量门限切换阶段，每个阶段使用不同弹幕模式。
 */
UCLASS()
class THUNDERFIGHTER_API ABossEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	ABossEnemy();

	virtual void Tick(float DeltaTime) override;

	/** 获取 Boss 当前血量百分比（0.0-1.0），供 HUD 血条使用 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Boss")
	float GetHealthPercent() const;

	/** 获取 Boss 当前最大血量，供 HUD 显示 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Boss")
	float GetMaxHealth() const;

protected:
	virtual void BeginPlay() override;

	/** 覆盖基类移动：Boss 使用入场 + 悬停扫射 */
	virtual void ApplyMovement(float DeltaTime) override;

	// ---- 阶段配置 ----

	/** 阶段列表，按血量门限从高到低排列（1.0 → 0.66 → 0.33） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Boss")
	TArray<FEnemyPhase> Phases;

	/** 入场速度（单位/秒，沿 +X 进入战场） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Boss")
	float EnterSpeed = 400.0f;

	/** 悬停的 X 位置（进入战场后停在此处） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Boss")
	float HoverX = 800.0f;

	/** 扫射 Y 方向半幅（左右摆动范围） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Boss")
	float StrafeAmplitude = 300.0f;

	/** 扫射摆动速度 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Boss")
	float StrafeSpeed = 0.8f;

	/** 弹幕蓝图类 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Boss")
	TSubclassOf<AProjectileBase> ProjectileClass;

private:
	/** 发射一波当前阶段的弹幕 */
	void FireBurst();

	/** 根据当前血量百分比刷新阶段索引 */
	void CheckPhaseTransition();

	/** 血量变化回调：用于阶段切换 */
	UFUNCTION()
	void OnBossHealthChanged(float CurrentHealth);

	/** 当前阶段索引 */
	int32 CurrentPhaseIndex = 0;

	/** 是否已完成入场 */
	bool bHasEntered = false;

	/** 进入战场时记录的 Y 原点（扫射基准） */
	float HoverYOrigin = 0.0f;

	/** Boss 专属射击计时器 */
	float BossFireTimer = 0.0f;
};
