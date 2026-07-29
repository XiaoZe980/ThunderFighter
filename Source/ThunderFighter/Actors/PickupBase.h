// ThunderFighter - 雷霆战机 PickupBase
// Base class for power-up pickups dropped by enemies

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UThunderFighterWeaponComponent;
class UThunderFighterHealthComponent;

/**
 * Type of pickup item.
 */
UENUM(BlueprintType)
enum class EPickupType : uint8
{
	WeaponUpgrade	UMETA(DisplayName = "Weapon Upgrade (W)"),
	HealthRestore	UMETA(DisplayName = "Health Restore (H)"),
	Bomb			UMETA(DisplayName = "Bomb (B)"),
	ScoreBonus		UMETA(DisplayName = "Score Bonus (S)")
};

/**
 * Pickup actor. Floats/drifts downward, and applies an effect when collected by the player.
 */
UCLASS()
class THUNDERFIGHTER_API APickupBase : public AActor
{
	GENERATED_BODY()

public:
	APickupBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// ---- Components ----

	/** Trigger sphere for pickup detection */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<USphereComponent> TriggerSphere;

	/** Visual mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

public:
	/** Initialize this pickup with a type and value */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Pickup")
	void Initialize(EPickupType InType, float InValue = 0.0f);

	/** Get the pickup type */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Pickup")
	EPickupType GetPickupType() const { return PickupType; }

protected:
	/** Called when player overlaps the trigger */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Apply the pickup effect to the player */
	void ApplyEffect(AActor* PlayerActor);

	/** Type of this pickup */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	EPickupType PickupType = EPickupType::WeaponUpgrade;

	/** Value (e.g. health amount, score points, weapon level increment) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	float Value = 1.0f;

	/** Float animation speed (bobbing) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	float FloatSpeed = 2.0f;

	/** Float amplitude */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	float FloatAmplitude = 10.0f;

	/** Downward drift speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	float DriftSpeed = 100.0f;

	/** Lifetime before auto-destroy (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	float Lifetime = 10.0f;

private:
	/** Initial Z position for float animation */
	float InitialZ = 0.0f;

	/** Lifetime accumulator */
	float LifetimeTimer = 0.0f;
};
