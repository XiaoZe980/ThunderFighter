// ThunderFighter - 雷霆战机 EnemySpawner 实现

#include "EnemySpawner.h"
#include "EnemyBase.h"
#include "Engine/World.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// 如果有预定义波次则自动开始生成
	if (Waves.Num() > 0)
	{
		StartSpawning();
	}
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSpawning)
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
	float OverrideHealth, float OverrideSpeed)
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
		// 只覆盖显式指定的值；Override 为 0 时保留敌人蓝图里配置的默认值
		Enemy->Initialize(OverrideSpeed, OverrideHealth);
	}

	return Enemy;
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
