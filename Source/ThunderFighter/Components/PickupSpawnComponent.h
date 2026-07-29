// ThunderFighter - 雷霆战机 PickupSpawnComponent
// Manages pickup spawning when enemies are defeated

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/PickupBase.h"
#include "PickupSpawnComponent.generated.h"

/**
 * Attach to GameMode or EnemyBase to handle pickup spawning on enemy death.
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UPickupSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPickupSpawnComponent();

	/**
	 * Try to spawn a pickup at the given location based on drop tables.
	 * @param Location World location to spawn at
	 * @param DropRateOverride Override the default drop rate (0.0 - 1.0). -1.0 uses defaults.
	 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Pickup")
	void TrySpawnPickup(FVector Location, float DropRateOverride = -1.0f);

	/**
	 * Spawn a specific pickup type at the location (always succeeds).
	 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Pickup")
	APickupBase* SpawnPickup(FVector Location, EPickupType Type, float Value = 0.0f);

	// ---- Configuration ----

	/** Base drop chance (0.0 - 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Pickup")
	float BaseDropRate = 0.3f;

	/** Pickup class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Pickup")
	TSubclassOf<APickupBase> PickupClass;

	/** Probability weights for each pickup type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Pickup")
	TMap<EPickupType, float> DropWeights;

protected:
	/** Get a random pickup type based on weights */
	EPickupType GetRandomPickupType();
};
