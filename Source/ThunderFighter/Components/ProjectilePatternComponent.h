// ThunderFighter - 雷霆战机 ProjectilePatternComponent
// Configurable bullet pattern emitter for enemies and bosses

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/ProjectileBase.h"
#include "ProjectilePatternComponent.generated.h"

/**
 * Attach to enemy actors to define their bullet firing patterns.
 * Supports multiple pattern types: single, spread, circle, spiral.
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UProjectilePatternComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProjectilePatternComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Fire a single projectile straight ahead */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void FireSingle(FVector Direction, float Speed, float Damage);

	/** Fire a spread (fan) of projectiles */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void FireSpread(int32 Count, float SpreadAngle, float Speed, float Damage);

	/** Fire projectiles in a full circle */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void FireCircle(int32 Count, float Speed, float Damage, float StartAngle = 0.0f);

	/** Fire towards a target position (aimed shot) */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void FireAtTarget(FVector TargetLocation, float Speed, float Damage, float Inaccuracy = 0.0f);

	/** Fire towards the player */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void FireAtPlayer(float Speed, float Damage, float Inaccuracy = 0.0f);

	/** Enable/disable automatic firing */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void SetAutoFire(bool bEnable, float FireInterval);

	/** Set the projectile class to spawn */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void SetProjectileClass(TSubclassOf<AProjectileBase> InClass) { ProjectileClass = InClass; }

protected:
	/** Spawn a single projectile */
	AProjectileBase* SpawnProjectile(FVector Direction, float Speed, float Damage);

	/** The faction for spawned projectiles */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Projectile")
	EProjectileFaction Faction = EProjectileFaction::Enemy;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Projectile")
	TSubclassOf<AProjectileBase> ProjectileClass;

	/** Random spread applied to each shot (degrees) */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Projectile")
	float BaseInaccuracy = 0.0f;

private:
	/** Auto-fire state */
	bool bAutoFire = false;

	/** Auto-fire interval */
	float AutoFireInterval = 0.5f;

	/** Auto-fire timer */
	float AutoFireTimer = 0.0f;
};
