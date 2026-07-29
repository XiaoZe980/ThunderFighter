// ThunderFighter - 雷霆战机 EnemySpawner
// Manages enemy wave spawning in the level

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyBase;

/**
 * Struct defining a single spawn entry in a wave.
 */
USTRUCT(BlueprintType)
struct FSpawnEntry
{
	GENERATED_BODY()

	/** Enemy class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	TSubclassOf<AEnemyBase> EnemyClass;

	/** Spawn position relative to spawner */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	FVector SpawnOffset = FVector(500.0f, 0.0f, 0.0f);

	/** Delay before spawning this entry (seconds from wave start) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	float SpawnDelay = 0.0f;

	/** Override health (0 = use default) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	float OverrideHealth = 0.0f;

	/** Override speed (0 = use default) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	float OverrideSpeed = 0.0f;
};

/**
 * Struct defining a wave of enemies.
 */
USTRUCT(BlueprintType)
struct FEnemyWave
{
	GENERATED_BODY()

	/** Display name for this wave */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	FString WaveName;

	/** Delay before this wave begins (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveStartDelay = 0.0f;

	/** Spawn entries in this wave */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray<FSpawnEntry> Entries;
};

/**
 * Spawner actor placed in the level. Controls enemy wave spawning.
 * Attach to GameMode or place directly in the level.
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
	/** Start spawning waves */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Spawner")
	void StartSpawning();

	/** Stop all spawning */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Spawner")
	void StopSpawning();

	/** Add a wave to the spawn queue */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Spawner")
	void AddWave(const FEnemyWave& Wave);

	/** Clear all pending waves */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Spawner")
	void ClearWaves();

	/** Spawn a single enemy at the given relative offset */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Spawner")
	AEnemyBase* SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass, FVector SpawnOffset,
		float OverrideHealth = 0.0f, float OverrideSpeed = 0.0f);

	/** Are we currently spawning? */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Spawner")
	bool IsSpawning() const { return bIsSpawning; }

protected:
	/** Process the current wave and spawn entries */
	void ProcessWaves(float DeltaTime);

	// ---- Configuration ----

	/** Pre-defined waves to spawn in sequence */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner")
	TArray<FEnemyWave> Waves;

	/** Loop waves after completing all? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner")
	bool bLoopWaves = false;

	/** Time between wave loops (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner")
	float LoopDelay = 5.0f;

	/** Spawn area half-width (Y axis) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner")
	float SpawnAreaWidth = 400.0f;

	/** Spawn area half-height (Z axis) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Spawner")
	float SpawnAreaHeight = 300.0f;

private:
	/** Is the spawner active? */
	bool bIsSpawning = false;

	/** Index into Waves array */
	int32 CurrentWaveIndex = 0;

	/** Timer for current wave delay */
	float WaveTimer = 0.0f;

	/** Timer for spawning within the current wave */
	float WaveElapsedTime = 0.0f;

	/** Track which entries in current wave have been spawned */
	TArray<bool> EntrySpawnedFlags;

	/** Loop cooldown timer */
	float LoopTimer = 0.0f;
};
