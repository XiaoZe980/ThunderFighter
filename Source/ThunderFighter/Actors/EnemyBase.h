// ThunderFighter - 雷霆战机 EnemyBase
// Base class for all enemy aircraft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

class UThunderFighterHealthComponent;
class UProjectilePatternComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UCurveFloat;
class UCurveVector;

/**
 * Base enemy class with health, scoring, and configurable movement patterns.
 * Derive in Blueprint for specific enemy types (grunt, shooter, rammer, boss).
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

	// ---- Components ----

	/** Collision box */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UBoxComponent> CollisionBox;

	/** Visual mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> EnemyMesh;

	/** Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UThunderFighterHealthComponent> HealthComponent;

	/** Projectile pattern component (optional, for enemies that shoot) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UProjectilePatternComponent> ProjectilePattern;

public:
	// ---- Movement ----

	/**
	 * Movement curve defining the enemy's path over time.
	 * X axis = time (seconds), Y axis = horizontal offset.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	TObjectPtr<UCurveFloat> MovementCurveX;

	/**
	 * Movement curve for vertical offset over time.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	TObjectPtr<UCurveFloat> MovementCurveY;

	/** Base movement speed multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	float MoveSpeedMultiplier = 1.0f;

	/** Starting direction (normalized). Default: move downward (-Y for top-down view) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	FVector BaseDirection = FVector(-1.0f, 0.0f, 0.0f);

	/** Base speed (units/sec) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	float BaseSpeed = 300.0f;

	// ---- Combat ----

	/** Score awarded on destruction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	int32 ScoreValue = 100;

	/** Whether this enemy can fire projectiles */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	bool bCanShoot = false;

	/** Fire interval when shooting (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	float FireInterval = 1.5f;

	/** Start firing after this many seconds on screen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	float FireStartDelay = 1.0f;

	/** Drop rate for pickups (0.0 - 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	float DropRate = 0.3f;

	// ---- Functions ----

	/** Initialize enemy with a specific speed override */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Enemy")
	void Initialize(float InSpeed, float InHealth);

	/** Set the base direction for movement */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Enemy")
	void SetBaseDirection(FVector NewDirection) { BaseDirection = NewDirection.GetSafeNormal(); }

protected:
	/** Called when health reaches zero */
	UFUNCTION()
	void OnEnemyDefeated();

	/** Apply movement for this frame based on curves or base direction */
	void ApplyMovement(float DeltaTime);

	/** Time since spawn */
	float AliveTime = 0.0f;

	/** Fire timer */
	float FireTimer = 0.0f;

	/** Has shooting started? */
	bool bHasStartedShooting = false;
};
