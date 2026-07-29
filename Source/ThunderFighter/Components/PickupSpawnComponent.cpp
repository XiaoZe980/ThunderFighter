// ThunderFighter - 雷霆战机 PickupSpawnComponent Implementation

#include "PickupSpawnComponent.h"
#include "Engine/World.h"

UPickupSpawnComponent::UPickupSpawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Default drop weights
	DropWeights.Add(EPickupType::WeaponUpgrade, 35.0f);
	DropWeights.Add(EPickupType::HealthRestore, 30.0f);
	DropWeights.Add(EPickupType::Bomb, 15.0f);
	DropWeights.Add(EPickupType::ScoreBonus, 20.0f);
}

void UPickupSpawnComponent::TrySpawnPickup(FVector Location, float DropRateOverride)
{
	float DropRate = DropRateOverride >= 0.0f ? DropRateOverride : BaseDropRate;

	if (FMath::FRand() > DropRate) return;

	EPickupType Type = GetRandomPickupType();
	SpawnPickup(Location, Type);
}

APickupBase* UPickupSpawnComponent::SpawnPickup(FVector Location, EPickupType Type, float Value)
{
	if (!PickupClass || !GetWorld()) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APickupBase* Pickup = GetWorld()->SpawnActor<APickupBase>(
		PickupClass, Location, FRotator::ZeroRotator, SpawnParams);

	if (Pickup)
	{
		Pickup->Initialize(Type, Value);
	}

	return Pickup;
}

EPickupType UPickupSpawnComponent::GetRandomPickupType()
{
	// Calculate total weight
	float TotalWeight = 0.0f;
	for (const auto& Pair : DropWeights)
	{
		TotalWeight += Pair.Value;
	}

	if (TotalWeight <= 0.0f) return EPickupType::ScoreBonus;

	float Random = FMath::FRandRange(0.0f, TotalWeight);
	float Accumulator = 0.0f;

	for (const auto& Pair : DropWeights)
	{
		Accumulator += Pair.Value;
		if (Random <= Accumulator)
		{
			return Pair.Key;
		}
	}

	return EPickupType::ScoreBonus;
}
