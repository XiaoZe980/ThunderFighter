// ThunderFighter - 雷霆战机 EnemySpawner 实现

#include "EnemySpawner.h"
#include "EnemyBase.h"
#include "BossEnemy.h"
#include "Components/ThunderFighterHealthComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// 如果有预定义波次或无尽模式则自动开始生成
	if (Waves.Num() > 0 || bEndlessMode)
	{
		StartSpawning();
	}
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsSpawning) return;

	if (bEndlessMode)
	{
		ProcessEndless(DeltaTime);
	}
	else
	{
		ProcessWaves(DeltaTime);
	}
}

void AEnemySpawner::StartSpawning()
{
	bIsSpawning = true;
	CurrentWaveIndex = 0;
	WaveTimer = 0.0f;
	WaveElapsedTime = 0.0f;
	EntrySpawnedFlags.Empty();

	// 重置无尽模式状态
	EndlessTimer = 0.0f;
	EndlessSpawnTimer = BaseSpawnInterval;
	NextBossTime = BossInterval;
	bBossActive = false;
}

void AEnemySpawner::StopSpawning()
{
	bIsSpawning = false;
}

void AEnemySpawner::AddWave(const FEnemyWave& Wave)
{
	Waves.Add(Wave);
}

void AEnemySpawner::ClearWaves()
{
	Waves.Empty();
	CurrentWaveIndex = 0;
	WaveTimer = 0.0f;
}

AEnemyBase* AEnemySpawner::SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass, FVector SpawnOffset,
	float OverrideHealth, float OverrideSpeed, float HealthMultiplier)
{
	if (!EnemyClass || !GetWorld()) return nullptr;

	FVector SpawnLocation = GetActorLocation() + SpawnOffset;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AEnemyBase* Enemy = GetWorld()->SpawnActor<AEnemyBase>(
		EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Enemy)
	{
		// 若指定血量倍率，读取默认血量并乘倍率（无尽模式难度递增用）
		if (HealthMultiplier > 1.0f)
		{
			if (UThunderFighterHealthComponent* HealthComp =
				Enemy->FindComponentByClass<UThunderFighterHealthComponent>())
			{
				float DefaultHealth = HealthComp->GetMaxHealth();
				Enemy->Initialize(OverrideSpeed, DefaultHealth * HealthMultiplier);
			}
			else
			{
				Enemy->Initialize(OverrideSpeed, OverrideHealth);
			}
		}
		else
		{
			// 只覆盖显式指定的值；Override 为 0 时保留敌人蓝图里配置的默认值
			Enemy->Initialize(OverrideSpeed, OverrideHealth);
		}
	}

	return Enemy;
}

void AEnemySpawner::ProcessEndless(float DeltaTime)
{
	EndlessTimer += DeltaTime;
	float Minutes = EndlessTimer / 60.0f;

	// 检测场景中是否还有 Boss 存活
	if (bBossActive)
	{
		bool bBossExists = false;
		for (TActorIterator<ABossEnemy> It(GetWorld()); It; ++It)
		{
			bBossExists = true;
			break;
		}
		if (!bBossExists)
		{
			bBossActive = false; // Boss 已被击败，下次可再生成
		}
	}

	// 难度参数：血量倍率随时间递增
	float HealthMultiplier = 1.0f + Minutes * HealthScalePerMinute;

	// 生成间隔随时间递减
	float SpawnInterval = FMath::Max(MinSpawnInterval,
		BaseSpawnInterval * FMath::Pow(0.9f, Minutes / FMath::Max(0.01f, IntervalDecayMinutes)));

	// Boss 调度：定时出现，在场时不重复生成
	if (BossClass && EndlessTimer >= NextBossTime)
	{
		if (!bBossActive)
		{
			bBossActive = true;
			FVector BossOffset = FVector(1200.0f, 0.0f, 0.0f);
			SpawnEnemy(BossClass, BossOffset, 0.0f, 0.0f, HealthMultiplier);
			UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] BOSS 出现! (%.0f 秒)"), EndlessTimer);
		}
		else
		{
			// Boss 还在场，等待死亡后再计时下一个
			NextBossTime = EndlessTimer + BossInterval;
		}
	}

	// 常规敌人生成
	EndlessSpawnTimer -= DeltaTime;
	if (EndlessSpawnTimer <= 0.0f)
	{
		if (EndlessEnemyPool.Num() > 0)
		{
			TSubclassOf<AEnemyBase> EnemyClass = EndlessEnemyPool[FMath::RandRange(0, EndlessEnemyPool.Num() - 1)];
			FVector Offset = FVector(1200.0f, FMath::FRandRange(-SpawnAreaWidth, SpawnAreaWidth), 0.0f);
			SpawnEnemy(EnemyClass, Offset, 0.0f, 0.0f, HealthMultiplier);
		}
		EndlessSpawnTimer = SpawnInterval;
	}
}

void AEnemySpawner::ProcessWaves(float DeltaTime)
{
	if (Waves.Num() == 0) return;

	// 循环计时器
	if (CurrentWaveIndex >= Waves.Num())
	{
		if (bLoopWaves)
		{
			LoopTimer -= DeltaTime;
			if (LoopTimer <= 0.0f)
			{
				CurrentWaveIndex = 0;
				WaveTimer = 0.0f;
				WaveElapsedTime = 0.0f;
				EntrySpawnedFlags.Empty();
				LoopTimer = LoopDelay;
			}
		}
		return;
	}

	const FEnemyWave& CurrentWave = Waves[CurrentWaveIndex];

	// 等待波次开始延迟
	if (WaveTimer < CurrentWave.WaveStartDelay)
	{
		WaveTimer += DeltaTime;
		return;
	}

	// 初始化生成追踪
	if (EntrySpawnedFlags.Num() != CurrentWave.Entries.Num())
	{
		EntrySpawnedFlags.Init(false, CurrentWave.Entries.Num());
		WaveElapsedTime = 0.0f;
	}

	WaveElapsedTime += DeltaTime;

	// 检查每个条目是否需要生成
	for (int32 i = 0; i < CurrentWave.Entries.Num(); i++)
	{
		if (EntrySpawnedFlags[i]) continue;

		const FSpawnEntry& Entry = CurrentWave.Entries[i];
		if (WaveElapsedTime >= Entry.SpawnDelay)
		{
			EntrySpawnedFlags[i] = true;

			// 在生成区域内随机化生成偏移
			FVector Offset = Entry.SpawnOffset;
			Offset.Y += FMath::FRandRange(-SpawnAreaWidth, SpawnAreaWidth);
			Offset.Z += FMath::FRandRange(-SpawnAreaHeight, SpawnAreaHeight);

			SpawnEnemy(Entry.EnemyClass, Offset, Entry.OverrideHealth, Entry.OverrideSpeed);
		}
	}

	// 检查此波次的所有条目是否都已生成
	bool bAllSpawned = true;
	for (bool b : EntrySpawnedFlags)
	{
		if (!b) { bAllSpawned = false; break; }
	}

	if (bAllSpawned)
	{
		CurrentWaveIndex++;
		WaveTimer = 0.0f;
		WaveElapsedTime = 0.0f;
		EntrySpawnedFlags.Empty();
		LoopTimer = LoopDelay;
	}
}
