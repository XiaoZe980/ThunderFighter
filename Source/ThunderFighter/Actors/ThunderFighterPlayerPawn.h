// ThunderFighter - 雷霆战机 PlayerPawn
// Player-controlled fighter aircraft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ThunderFighterPlayerPawn.generated.h"

class UThunderFighterHealthComponent;
class UThunderFighterWeaponComponent;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;

/**
 * Player fighter pawn. Controlled via EnhancedInput through PlayerController.
 * Handles movement within screen bounds, auto-firing, and bomb usage.
 */
UCLASS()
class THUNDERFIGHTER_API AThunderFighterPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AThunderFighterPlayerPawn();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// ---- Components ----

	/** Root collision box for the player ship */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UBoxComponent> CollisionBox;

	/** Static mesh or sprite for the player ship */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> ShipMesh;

	/** Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UThunderFighterHealthComponent> HealthComponent;

	/** Weapon component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UThunderFighterWeaponComponent> WeaponComponent;

	/** Spring arm for top-down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Camera")
	TObjectPtr<USpringArmComponent> CameraSpringArm;

	/** Top-down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

public:
	// ---- Movement ----

	/** Add world-space movement input (clamped to screen bounds) */
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	/** Movement speed (units/sec) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	float MoveSpeed = 1200.0f;

	/** Screen boundary margin (percentage of viewport, 0.0-0.5) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	float ScreenBoundaryMargin = 0.05f;

	// ---- Firing ----

	/** Start auto-firing */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Combat")
	void StartFiring();

	/** Stop auto-firing */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Combat")
	void StopFiring();

	/** Use a bomb (screen-clearing attack) */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Combat")
	void UseBomb();

	/** Is the player currently firing? */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Combat")
	bool IsFiring() const { return bIsFiring; }

	/** If true, the player fires automatically (default for STG games) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	bool bAutoFireEnabled = true;

	// ---- Invincibility ----

	/** Grant temporary invincibility (e.g. after taking damage) */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Combat")
	void GrantInvincibility(float Duration);

	/** Is the player currently invincible? */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Combat")
	bool IsInvincible() const { return bIsInvincible; }

protected:
	/** Called when health reaches zero */
	UFUNCTION()
	void OnHealthDepleted();

	/** Clamp the player position to screen bounds */
	void ClampToScreenBounds();

	/** Get screen bounds in world space */
	void GetScreenWorldBounds(float& OutMinX, float& OutMaxX, float& OutMinY, float& OutMaxY) const;

	/** Firing state */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Combat")
	bool bIsFiring = false;

	/** Invincibility flag */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Combat")
	bool bIsInvincible = false;

	/** Invincibility timer */
	float InvincibilityTimer = 0.0f;
};
