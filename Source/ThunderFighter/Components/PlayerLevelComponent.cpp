// ThunderFighter - 雷霆战机 PlayerLevelComponent 实现

#include "PlayerLevelComponent.h"

UPlayerLevelComponent::UPlayerLevelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerLevelComponent::AddExperience(float Amount)
{
	if (Amount <= 0.0f) return;

	Experience += Amount;
	CheckLevelUp();
}

void UPlayerLevelComponent::CheckLevelUp()
{
	// 可能一次获得大量经验连续升多级
	while (Experience >= ExperienceToNext)
	{
		Experience -= ExperienceToNext;
		Level++;

		// 下一级所需经验按增长倍率提升
		ExperienceToNext = BaseXPToNext * FMath::Pow(XPGrowthFactor, (float)(Level - 1));

		UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Level up! Now level %d (next: %.0f XP)"),
			Level, ExperienceToNext);

		// 广播升级事件
		OnLevelUp.Broadcast();
	}
}
