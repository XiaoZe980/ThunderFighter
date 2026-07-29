// ThunderFighter - 雷霆战机 ProjectileBase
// Base class for all projectiles (player and enemy bullets)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

/**
 * Faction identifier for projectiles.
 * Determines which actors a projectile can damage.
 */
UENUM(BlueprintType)
enum class EProjectileFaction : uint8
{
	Player	UMETA(DisplayName = "Player"),
	Enemy	UMETA(DisplayName = "Enemy")
};

/**
 * Base projectile class. Moves in a straight line at constant speed.
 * Can damage actors of the opposing faction on collision.
 */
UCLASS()
class THUNDERFIGHTER_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

	virtual void Tick(float DeltaTime) override;

	/**
	 * Initialize the projectile after spawning.
	 * @param InSpeed Movement speed (units/sec)
	 * @param InDamage Damage dealt on hit
	 * @param InFaction Which side this projectile belongs to
	 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void Initialize(float InSpeed, float InDamage, EProjectileFaction InFaction);

	/** Get the faction of this projectile */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Projectile")
	EProjectileFaction GetFaction() const { return Faction; }

	/** Get the damage value */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Projectile")
	float GetDamage() const { return Damage; }

	/** Set projectile lifetime (auto-destroy after this many seconds) */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void SetLifetime(float InLifetime);

protected:
	virtual void BeginPlay() override;

	/** Called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Deactivate and return this projectile to pool (or destroy) */
	void DeactivateProjectile();

	// ---- Components ----

	/** Sphere collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<USphereComponent> CollisionSphere;

	/** Visual mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	/** Projectile movement */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	// ---- Properties ----

	/** Projectile speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Projectile")
	float Speed = 2000.0f;

	/** Damage dealt on hit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Projectile")
	float Damage = 10.0f;

	/** Which side this projectile belongs to */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Projectile")
	EProjectileFaction Faction = EProjectileFaction::Player;

	/** Lifetime before auto-destroy (0 = infinite, managed externally) */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Projectile")
	float MaxLifetime = 5.0f;

	/** Current lifetime accumulator */
	float LifetimeTimer = 0.0f;
};
