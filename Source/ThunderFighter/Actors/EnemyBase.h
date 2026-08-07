// ThunderFighter - 雷霆战机 EnemyBase
// 所有敌方战机的基类

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

class UThunderFighterHealthComponent;
class UProjectilePatternComponent;
class UPickupSpawnComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UCurveFloat;
class UCurveVector;
class AEnergyCrystal;

/**
 * 敌方基类，具有生命值、计分和可配置的移动模式。
 * 在蓝图中派生出特定的敌人类型（小兵、射手、冲撞者、Boss）。
 */
UCLASS()
class THUNDERFIGHTER_API AEnemyBase : public AActor
{
	GENERATED_BODY()

public:
	AEnemyBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// ---- 组件 ----

	/** 碰撞盒 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UBoxComponent> CollisionBox;

	/** 视觉网格体 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> EnemyMesh;

	/** 生命值组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UThunderFighterHealthComponent> HealthComponent;

	/** 弹幕模式组件（可选，用于会射击的敌人） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UProjectilePatternComponent> ProjectilePattern;

	/** 道具掉落组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UPickupSpawnComponent> PickupSpawn;

public:
	// ---- 移动 ----

	/**
	 * 定义敌人随时间运动路径的移动曲线。
	 * X 轴 = 时间（秒），Y 轴 = 水平偏移。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	TObjectPtr<UCurveFloat> MovementCurveX;

	/**
	 * 随时间变化的垂直偏移移动曲线。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	TObjectPtr<UCurveFloat> MovementCurveY;

	/** 基础移动速度倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	float MoveSpeedMultiplier = 1.0f;

	/** 初始方向（归一化）。默认：向下移动（俯视视角下为 -Y） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	FVector BaseDirection = FVector(-1.0f, 0.0f, 0.0f);

	/** 基础速度（单位/秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	float BaseSpeed = 300.0f;

	// ---- 战斗 ----

	/** 被摧毁时奖励的分数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	int32 ScoreValue = 100;

	/** 此敌人是否可以发射弹幕 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	bool bCanShoot = false;

	/** 射击间隔（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	float FireInterval = 1.5f;

	/** 出现在屏幕上此秒数后开始射击 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	float FireStartDelay = 1.0f;

	/** 道具掉落率（-1 = 使用 PickupSpawn 组件的默认值，0.0-1.0 = 覆盖） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	float DropRate = -1.0f;

	/** 碰到玩家时对玩家造成的伤害 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	float CollisionDamage = 20.0f;

	/** 死亡时掉落的经验球蓝图类 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	TSubclassOf<AEnergyCrystal> EnergyCrystalClass;

	/** 死亡时掉落经验球提供的经验值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	float ExpValue = 5.0f;

	// ---- 函数 ----

	/** 使用指定速度覆盖来初始化敌人 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Enemy")
	void Initialize(float InSpeed, float InHealth);

	/** 设置移动的基础方向 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Enemy")
	void SetBaseDirection(FVector NewDirection) { BaseDirection = NewDirection.GetSafeNormal(); }

protected:
	/** 生命值归零时调用 */
	UFUNCTION()
	void OnEnemyDefeated();

	/** 根据曲线或基础方向应用本帧移动（Boss 可覆盖实现专属移动） */
	virtual void ApplyMovement(float DeltaTime);

	/** 生成后已存活时间 */
	float AliveTime = 0.0f;

	/** 射击计时器 */
	float FireTimer = 0.0f;

	/** 是否已开始射击？ */
	bool bHasStartedShooting = false;
};
