// ThunderFighter - 雷霆战机 HealthComponent 实现

#include "ThunderFighterHealthComponent.h"

UThunderFighterHealthComponent::UThunderFighterHealthComponent()
{
	// 需要 Tick 处理再生回血
	PrimaryComponentTick.bCanEverTick = true;
}

void UThunderFighterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	Shield = 0.0f;
	bIsDead = false;
}

void UThunderFighterHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 再生回血
	if (RegenerationPerSecond > 0.0f && !bIsDead)
	{
		Heal(RegenerationPerSecond * DeltaTime);
	}
}

void UThunderFighterHealthComponent::TakeDamage(float DamageAmount)
{
	if (bIsDead || DamageAmount <= 0.0f) return;

	// 先扣护盾
	if (Shield > 0.0f)
	{
		float ShieldAbsorb = FMath::Min(Shield, DamageAmount);
		Shield -= ShieldAbsorb;
		DamageAmount -= ShieldAbsorb;

		if (DamageAmount <= 0.0f)
		{
			OnDamageTaken.Broadcast(ShieldAbsorb);
			return; // 护盾完全吸收
		}
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);

	OnHealthChanged.Broadcast(CurrentHealth);
	OnDamageTaken.Broadcast(DamageAmount);

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] %s 受到 %.1f 点伤害，剩余血量: %.1f/%.1f"),
		*GetOwner()->GetName(), DamageAmount, CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f && !bIsDead)
	{
		bIsDead = true;
		UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] %s 被摧毁!"), *GetOwner()->GetName());
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

void UThunderFighterHealthComponent::GrantShield(float ShieldAmount)
{
	Shield += ShieldAmount;
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] %s 获得护盾 %.1f（当前 %.1f）"),
		*GetOwner()->GetName(), ShieldAmount, Shield);
}

void UThunderFighterHealthComponent::SetRegeneration(float PerSecond)
{
	RegenerationPerSecond = FMath::Max(0.0f, PerSecond);
}
