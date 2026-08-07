// ThunderFighter - 雷霆战机 UpgradeSystem 实现

#include "UpgradeSystem.h"
#include "Actors/ThunderFighterPlayerPawn.h"
#include "Components/ThunderFighterWeaponComponent.h"
#include "Components/ThunderFighterHealthComponent.h"
#include "Components/PlayerLevelComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

UUpgradeSystem::UUpgradeSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

int32 UUpgradeSystem::GetUpgradeStacks(FName UpgradeID) const
{
	const int32* Found = UpgradeStacks.Find(UpgradeID);
	return Found ? *Found : 0;
}

TArray<FUpgradeDefinition> UUpgradeSystem::DrawOptions()
{
	TArray<FUpgradeDefinition> Result;

	if (!UpgradePool) return Result;

	// 收集所有可选强化（剔除已达叠加上限的）
	TArray<FUpgradeDefinition> Available;
	static const FString ContextString(TEXT("DrawUpgrade"));
	for (const TPair<FName, uint8*>& Row : UpgradePool->GetRowMap())
	{
		if (FUpgradeDefinition* Up = reinterpret_cast<FUpgradeDefinition*>(Row.Value))
		{
			int32 CurrentStacks = GetUpgradeStacks(Up->UpgradeID);
			if (CurrentStacks < Up->MaxStacks)
			{
				Available.Add(*Up);
			}
		}
	}

	if (Available.Num() == 0) return Result;

	// 按权重抽取，避免重复
	int32 Count = FMath::Min(OptionsCount, Available.Num());
	for (int32 i = 0; i < Count; i++)
	{
		// 计算剩余可选的总权重
		float TotalWeight = 0.0f;
		for (const FUpgradeDefinition& Up : Available)
		{
			TotalWeight += Up.Weight;
		}

		if (TotalWeight <= 0.0f) break;

		float Random = FMath::FRandRange(0.0f, TotalWeight);
		float Accumulator = 0.0f;
		int32 SelectedIndex = 0;

		for (int32 j = 0; j < Available.Num(); j++)
		{
			Accumulator += Available[j].Weight;
			if (Random <= Accumulator)
			{
				SelectedIndex = j;
				break;
			}
		}

		Result.Add(Available[SelectedIndex]);
		Available.RemoveAt(SelectedIndex);
	}

	return Result;
}

void UUpgradeSystem::ApplyUpgrade(const FUpgradeDefinition& Upgrade)
{
	// 记录叠加层数
	int32& Stacks = UpgradeStacks.FindOrAdd(Upgrade.UpgradeID);
	Stacks++;

	// 获取玩家及其组件
	AThunderFighterPlayerPawn* Player = Cast<AThunderFighterPlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!Player) return;

	UThunderFighterWeaponComponent* Weapon = Player->FindComponentByClass<UThunderFighterWeaponComponent>();
	UThunderFighterHealthComponent* Health = Player->FindComponentByClass<UThunderFighterHealthComponent>();
	UPlayerLevelComponent* Level = Player->FindComponentByClass<UPlayerLevelComponent>();

	const float V = Upgrade.EffectValue;

	// 根据效果类型分发
	switch (Upgrade.Effect)
	{
		// ---- 武器类 ----
		case EUpgradeEffect::BonusBullets:
			if (Weapon) Weapon->ApplyModifier(EUpgradeEffect::BonusBullets, V);
			break;
		case EUpgradeEffect::DamageMultiplier:
			if (Weapon) Weapon->ApplyModifier(EUpgradeEffect::DamageMultiplier, V);
			break;
		case EUpgradeEffect::FireRateMultiplier:
			if (Weapon) Weapon->ApplyModifier(EUpgradeEffect::FireRateMultiplier, V);
			break;
		case EUpgradeEffect::Piercing:
			if (Weapon) Weapon->ApplyModifier(EUpgradeEffect::Piercing, V);
			break;
		case EUpgradeEffect::Homing:
			if (Weapon) Weapon->ApplyModifier(EUpgradeEffect::Homing, V);
			break;
		case EUpgradeEffect::Bounce:
			if (Weapon) Weapon->ApplyModifier(EUpgradeEffect::Bounce, V);
			break;
		case EUpgradeEffect::SideWings:
			if (Weapon) Weapon->ApplyModifier(EUpgradeEffect::SideWings, V);
			break;

		// ---- 生存类 ----
		case EUpgradeEffect::MaxHealth:
			if (Health)
			{
				Health->SetMaxHealth(Health->GetMaxHealth() + V);
				Health->Heal(V); // 同时回复
			}
			break;
		case EUpgradeEffect::Shield:
			if (Health) Health->GrantShield(V);
			break;
		case EUpgradeEffect::Regeneration:
			if (Health) Health->SetRegeneration(Health->GetRegeneration() + V);
			break;
		case EUpgradeEffect::InvincibilityTime:
			Player->InvincibilityDuration += V;
			break;

		// ---- 移动类 ----
		case EUpgradeEffect::MoveSpeed:
			Player->MoveSpeed += V;
			break;

		// ---- 特殊类 ----
		case EUpgradeEffect::BombCount:
			if (Weapon) Weapon->AddBomb((int32)V);
			break;
		case EUpgradeEffect::ExpGainMultiplier:
			if (Level) Level->SetExpMultiplier(Level->GetExpMultiplier() + V);
			break;
		case EUpgradeEffect::MagnetRadius:
			break; // 预留：经验球吸附范围（后续实现）

		default:
			break;
	}

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] 强化应用: %s (%s x%.2f) 层数=%d"),
		*Upgrade.DisplayName, *UEnum::GetValueAsString(Upgrade.Effect), V, Stacks);
}
