// ThunderFighter - 雷霆战机 HealthComponent Implementation

#include "ThunderFighterHealthComponent.h"

UThunderFighterHealthComponent::UThunderFighterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UThunderFighterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	bIsDead = false;
}

void UThunderFighterHealthComponent::TakeDamage(float DamageAmount)
{
	if (bIsDead || DamageAmount <= 0.0f) return;

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);

	OnHealthChanged.Broadcast(CurrentHealth);
	OnDamageTaken.Broadcast(DamageAmount);

	UE_LOG(LogTemp, Verbose, TEXT("[ThunderFighter] %s took %.1f damage. Health: %.1f/%.1f"),
		*GetOwner()->GetName(), DamageAmount, CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f && !bIsDead)
	{
		bIsDead = true;
		OnHealthDepleted.Broadcast();
	}
}

void UThunderFighterHealthComponent::Heal(float HealAmount)
{
	if (bIsDead || HealAmount <= 0.0f) return;

	CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + HealAmount);
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UThunderFighterHealthComponent::SetMaxHealth(float NewMaxHealth, bool bResetCurrentHealth)
{
	MaxHealth = FMath::Max(1.0f, NewMaxHealth);
	if (bResetCurrentHealth)
	{
		CurrentHealth = MaxHealth;
		OnHealthChanged.Broadcast(CurrentHealth);
	}
}
