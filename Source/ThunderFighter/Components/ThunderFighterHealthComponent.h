// ThunderFighter - 雷霆战机 HealthComponent
// Reusable health management component for both player and enemies

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThunderFighterHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTaken, float, DamageAmount);

/**
 * Component that manages health, damage, and death for any actor.
 * Attach to player ships, enemies, or destructible objects.
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UThunderFighterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UThunderFighterHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** Apply damage to this actor */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void TakeDamage(float DamageAmount);

	/** Heal by the specified amount */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void Heal(float HealAmount);

	/** Is this actor still alive? */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	bool IsAlive() const { return CurrentHealth > 0.0f; }

	/** Get current health percentage (0.0 - 1.0) */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetHealthPercent() const { return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f; }

	/** Get current health value */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	/** Get max health */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetMaxHealth() const { return MaxHealth; }

	/** Set max health and optionally reset current health */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void SetMaxHealth(float NewMaxHealth, bool bResetCurrentHealth = true);

	// ---- Delegates ----

	/** Fired when health reaches zero */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Health")
	FOnHealthDepleted OnHealthDepleted;

	/** Fired whenever health changes */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Health")
	FOnHealthChanged OnHealthChanged;

	/** Fired when damage is taken */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Health")
	FOnDamageTaken OnDamageTaken;

protected:
	/** Maximum health */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Health")
	float MaxHealth = 100.0f;

	/** Current health */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Health")
	float CurrentHealth = 100.0f;

	/** Is this actor dead? (prevents multiple death triggers) */
	bool bIsDead = false;
};
