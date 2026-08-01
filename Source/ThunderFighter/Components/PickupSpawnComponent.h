// ThunderFighter - 雷霆战机 PickupSpawnComponent
// 管理敌人被击败时的拾取道具生成

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/PickupBase.h"
#include "PickupSpawnComponent.generated.h"

/**
 * 附加到 GameMode 或 EnemyBase 上，处理敌人死亡时的拾取道具生成。
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UPickupSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPickupSpawnComponent();

	/**
	 * 尝试根据掉落表在给定位置生成拾取道具。
	 * @param Location 生成的世界坐标位置
	 * @param DropRateOverride 覆盖默认掉落率（0.0 - 1.0）。-1.0 使用默认值。
	 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Pickup")
	void TrySpawnPickup(FVector Location, float DropRateOverride = -1.0f);

	/**
	 * 在指定位置生成特定类型的拾取道具（始终成功）。
	 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Pickup")
	APickupBase* SpawnPickup(FVector Location, EPickupType Type, float Value = 0.0f);

	// ---- 配置 ----

	/** 基础掉落概率（0.0 - 1.0） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Pickup")
	float BaseDropRate = 0.3f;

	/** 每种道具类型对应的蓝图类（Key=类型, Value=蓝图） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Pickup")
	TMap<EPickupType, TSubclassOf<APickupBase>> PickupClassMap;

	/** 每种拾取道具类型的概率权重 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Pickup")
	TMap<EPickupType, float> DropWeights;

protected:
	/** 根据权重获取随机拾取道具类型 */
	EPickupType GetRandomPickupType();
};
