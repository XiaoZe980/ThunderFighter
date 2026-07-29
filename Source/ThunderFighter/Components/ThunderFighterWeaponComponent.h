// ThunderFighter - 雷霆战机 WeaponComponent
// Manages shooting logic, bullet spawning, and weapon levels for the player

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThunderFighterWeaponComponent.generated.h"

class AProjectileBase;

/**
 * Attached to the player pawn. Manages weapon level, fire rate,
 * and projectile spawning patterns.
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UThunderFighterWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UThunderFighterWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Start continuous firing */
	void StartFiring();

	/** Stop continuous firing */
	void StopFiring();

	/** Activate bomb (screen-clearing attack) */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Weapon")
	void ActivateBomb();

	/** Add a bomb to the player's inventory */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Weapon")
	void AddBomb(int32 Count = 1);

	/** Increase weapon level by 1 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Weapon")
	void UpgradeWeapon();

	/** Get current weapon level */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Weapon")
	int32 GetWeaponLevel() const { return WeaponLevel; }

	/** Get current bomb count */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Weapon")
	int32 GetBombCount() const { return BombCount; }

protected:
	/** Spawn a projectile at the given local offset */
	void FireProjectile(FVector LocalOffset);

	/** Fire according to current weapon level pattern */
	void FirePattern();

	// ---- Configurable Properties ----

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	TSubclassOf<AProjectileBase> ProjectileClass;

	/** Fire rate (shots per second) at level 1 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	float BaseFireRate = 5.0f;

	/** Current weapon level (1-5) */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	int32 WeaponLevel = 1;

	/** Maximum weapon level */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	int32 MaxWeaponLevel = 5;

	/** Projectile speed (units/sec) */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	float ProjectileSpeed = 2000.0f;

	/** Projectile damage */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	float ProjectileDamage = 10.0f;

	/** Number of bombs available */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	int32 BombCount = 3;

	/** Spawn offsets for level 1 (single shot center) */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon|Patterns")
	TArray<FVector> PatternLevel1;

	/** Spawn offsets for level 2 (twin shot) */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon|Patterns")
	TArray<FVector> PatternLevel2;

	/** Spawn offsets for level 3 (triple shot) */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon|Patterns")
	TArray<FVector> PatternLevel3;

	/** Spawn offsets for level 4 (quad shot) */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon|Patterns")
	TArray<FVector> PatternLevel4;

	/** Spawn offsets for level 5 (full spread) */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon|Patterns")
	TArray<FVector> PatternLevel5;

private:
	/** Is the weapon currently firing? */
	bool bIsFiring = false;

	/** Time accumulator for fire rate */
	float FireTimer = 0.0f;
};
