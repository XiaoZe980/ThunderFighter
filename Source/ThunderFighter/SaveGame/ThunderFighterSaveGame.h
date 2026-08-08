// ThunderFighter - 雷霆战机 ThunderFighterSaveGame
// 局外养成数据的持久化存档（金币 + 永久升级）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ThunderFighterSaveGame.generated.h"

/**
 * 保存局外养成数据：金币数量和各项永久升级等级。
 * 使用 USaveGame 系统持久化到磁盘。
 */
UCLASS()
class THUNDERFIGHTER_API UThunderFighterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** 金币数量 */
	UPROPERTY(VisibleAnywhere, Category = "Save")
	int32 Gold = 0;

	/** 伤害永久升级等级 */
	UPROPERTY(VisibleAnywhere, Category = "Save")
	int32 DamageUpLevel = 0;

	/** 血量永久升级等级 */
	UPROPERTY(VisibleAnywhere, Category = "Save")
	int32 HealthUpLevel = 0;

	/** 射速永久升级等级 */
	UPROPERTY(VisibleAnywhere, Category = "Save")
	int32 FireRateUpLevel = 0;

	/** 移速永久升级等级 */
	UPROPERTY(VisibleAnywhere, Category = "Save")
	int32 SpeedUpLevel = 0;
};
