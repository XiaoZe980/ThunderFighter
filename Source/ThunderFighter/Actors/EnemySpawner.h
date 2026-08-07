// ThunderFighter - 雷霆战机 EnemySpawner
// 管理关卡中的敌人波次生成

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyBase;

/**
 * 定义波次中单个生成条目的结构体。
 */
USTRUCT(BlueprintType)
struct FSpawnEntry
{
	GENERATED_BODY()

	/** 要生成的敌人类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	TSubclassOf<AEnemyBase> EnemyClass;

	/** 相对于生成器的生成位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	FVector SpawnOffset = FVector(500.0f, 0.0f, 0.0f);

	/** 生成此条目前的延迟（从波次开始的秒数） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	float SpawnDelay = 0.0f;

	/** 覆盖生命值（0 = 使用默认值） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	float OverrideHealth = 0.0f;

	/** 覆盖速度（0 = 使用默认值） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	float OverrideSpeed = 0.0f;
};

/**
 * 定义一波敌人的结构体。
 */
USTRUCT(BlueprintType)
struct FEnemyWave
{
	GENERATED_BODY()

	/** 此波次的显示名称 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	FString WaveName;

	/** 此波次开始前的延迟（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveStartDelay = 0.0f;

	/** 此波次中的生成条目 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray<FSpawnEntry> Entries;
};

/**
 * 放置在关卡中的生成器 Actor。控制敌人波次生成。
 * 可附加到 GameMode 或直接放置在关卡中。
 */
UCLASS()
class THUNDERFIGHTER_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	/** 开始生成波次 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Spawner")
	void StartSpawning();

	/** 停止所有生成 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Spawner")
	void StopSpawning();

	/** 向生成队列添加一个波次 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Spawner")
	void AddWave(const FEnemyWave& Wave);

	/** 清除所有待生成的波次 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Spawner")
	void ClearWaves();

	/** 在给定的相对偏移位置生成单个敌人 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Spawner")
	AEnemyBase* SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass, FVector SpawnOffset,
		float OverrideHealth = 0.0f, float OverrideSpeed = 0.0f, float HealthMultiplier = 1.0f);

	/** 当前是否正在生成？ */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Spawner")
	bool IsSpawning() const { return bIsSpawning; }

protected:
	/** 处理当前波次并生成条目 */
	void ProcessWaves(float DeltaTime);

	/** 处理无尽模式生成（Roguelike 难度递增） */
	void ProcessEndless(float DeltaTime);

	// ---- 配置 ----

	/** 按顺序生成的预定义波次 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner")
	TArray<FEnemyWave> Waves;

	/** 完成所有波次后是否循环？ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner")
	bool bLoopWaves = false;

	/** 波次循环之间的间隔时间（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner")
	float LoopDelay = 5.0f;

	/** 生成区域半宽（Y 轴） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner")
	float SpawnAreaWidth = 400.0f;

	/** 生成区域半高（Z 轴） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner")
	float SpawnAreaHeight = 300.0f;

	// ---- 无尽模式（Roguelike） ----

	/** 启用无尽模式（基于时间持续生成，难度递增） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner|Endless")
	bool bEndlessMode = true;

	/** 基础生成间隔（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner|Endless")
	float BaseSpawnInterval = 2.0f;

	/** 生成间隔最短值（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner|Endless")
	float MinSpawnInterval = 0.5f;

	/** 每过多少分钟生成间隔降低 10% */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner|Endless")
	float IntervalDecayMinutes = 1.0f;

	/** 每过一分钟敌人血量增加比例（0.5 = 每分钟 +50%） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner|Endless")
	float HealthScalePerMinute = 0.5f;

	/** 无尽模式可生成的敌人类型池 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner|Endless")
	TArray<TSubclassOf<AEnemyBase>> EndlessEnemyPool;

	/** Boss 敌人类型（定期生成） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner|Endless")
	TSubclassOf<AEnemyBase> BossClass;

	/** Boss 出现间隔（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner|Endless")
	float BossInterval = 60.0f;

private:
	/** 生成器是否处于激活状态？ */
	bool bIsSpawning = false;

	/** Waves 数组的索引 */
	int32 CurrentWaveIndex = 0;

	/** 当前波次延迟计时器 */
	float WaveTimer = 0.0f;

	/** 当前波次内的生成计时器 */
	float WaveElapsedTime = 0.0f;

	/** 记录当前波次中哪些条目已生成 */
	TArray<bool> EntrySpawnedFlags;

	/** 循环冷却计时器 */
	float LoopTimer = 0.0f;

	/** 无尽模式累计时间（秒） */
	float EndlessTimer = 0.0f;

	/** 无尽模式常规生成计时器 */
	float EndlessSpawnTimer = 0.0f;

	/** 下次 Boss 出现时间 */
	float NextBossTime = 0.0f;

	/** 当前 Boss 是否在场（等它死亡再生成下一个） */
	bool bBossActive = false;
};
