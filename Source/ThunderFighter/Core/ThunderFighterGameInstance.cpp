// ThunderFighter - 雷霆战机 GameInstance 实现

#include "ThunderFighterGameInstance.h"
#include "SaveGame/ThunderFighterSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UThunderFighterGameInstance::Init()
{
	Super::Init();

	// 加载局外养成存档
	LoadSaveData();

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] GameInstance initialized"));
}

bool UThunderFighterGameInstance::TryUpdateHighScore(int32 NewScore)
{
	if (NewScore > HighScore)
	{
		HighScore = NewScore;
		return true;
	}
	return false;
}

void UThunderFighterGameInstance::ResetGameState()
{
	Lives = 3;
	BombCount = 3;
	WeaponLevel = 1;
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Game state reset"));
}

// ---- 局外养成 ----

void UThunderFighterGameInstance::LoadSaveData()
{
	SaveGameData = Cast<UThunderFighterSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ThunderFighterSave"), 0));

	if (!SaveGameData)
	{
		// 无存档，创建默认
		SaveGameData = Cast<UThunderFighterSaveGame>(UGameplayStatics::CreateSaveGameObject(UThunderFighterSaveGame::StaticClass()));
	}

	if (SaveGameData)
	{
		Gold = SaveGameData->Gold;

		PermanentUpgradeLevels.Empty();
		PermanentUpgradeLevels.Add(EPermanentUpgradeType::Damage, SaveGameData->DamageUpLevel);
		PermanentUpgradeLevels.Add(EPermanentUpgradeType::Health, SaveGameData->HealthUpLevel);
		PermanentUpgradeLevels.Add(EPermanentUpgradeType::FireRate, SaveGameData->FireRateUpLevel);
		PermanentUpgradeLevels.Add(EPermanentUpgradeType::Speed, SaveGameData->SpeedUpLevel);

		UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] 存档加载: 金币=%d"), Gold);
	}
}

void UThunderFighterGameInstance::SaveData()
{
	if (!SaveGameData)
	{
		SaveGameData = Cast<UThunderFighterSaveGame>(UGameplayStatics::CreateSaveGameObject(UThunderFighterSaveGame::StaticClass()));
	}

	if (!SaveGameData) return;

	SaveGameData->Gold = Gold;
	SaveGameData->DamageUpLevel = GetPermanentUpgradeLevel(EPermanentUpgradeType::Damage);
	SaveGameData->HealthUpLevel = GetPermanentUpgradeLevel(EPermanentUpgradeType::Health);
	SaveGameData->FireRateUpLevel = GetPermanentUpgradeLevel(EPermanentUpgradeType::FireRate);
	SaveGameData->SpeedUpLevel = GetPermanentUpgradeLevel(EPermanentUpgradeType::Speed);

	UGameplayStatics::SaveGameToSlot(SaveGameData, TEXT("ThunderFighterSave"), 0);
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] 存档已保存: 金币=%d"), Gold);
}

void UThunderFighterGameInstance::AddGold(int32 Amount)
{
	Gold += Amount;
}

bool UThunderFighterGameInstance::SpendGold(int32 Amount)
{
	if (Gold < Amount) return false;

	Gold -= Amount;
	return true;
}

int32 UThunderFighterGameInstance::GetPermanentUpgradeLevel(EPermanentUpgradeType Type) const
{
	const int32* Found = PermanentUpgradeLevels.Find(Type);
	return Found ? *Found : 0;
}

int32 UThunderFighterGameInstance::GetUpgradeCost(EPermanentUpgradeType Type) const
{
	int32 Level = GetPermanentUpgradeLevel(Type);
	// 价格 = 基础价 * 增长倍率^等级
	return FMath::RoundToInt(BaseUpgradeCost * FMath::Pow(CostGrowthPerLevel, (float)Level));
}

bool UThunderFighterGameInstance::UpgradePermanent(EPermanentUpgradeType Type)
{
	int32 Cost = GetUpgradeCost(Type);
	if (!SpendGold(Cost)) return false;

	int32& Level = PermanentUpgradeLevels.FindOrAdd(Type);
	Level++;
	SaveData();

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] 永久升级 %d 到 Lv.%d (花费 %d 金币)"),
		(int32)Type, Level, Cost);
	return true;
}
