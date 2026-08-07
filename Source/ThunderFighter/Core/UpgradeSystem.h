// ThunderFighter - 雷霆战机 UpgradeSystem
// 强化卡系统的抽取与应用逻辑

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/UpgradeTypes.h"
#include "UpgradeSystem.generated.h"

class UDataTable;

/**
 * 挂在 GameMode 上。管理强化卡的抽取与应用。
 * 从 DataTable（DT_Upgrades）中按权重抽取候选强化，应用时修改玩家武器/属性。
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UUpgradeSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UUpgradeSystem();

	/** 从强化池抽取 3 个候选（按权重，剔除已达叠加上限的） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Upgrade")
	TArray<FUpgradeDefinition> DrawOptions();

	/** 应用一个强化到玩家 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Upgrade")
	void ApplyUpgrade(const FUpgradeDefinition& Upgrade);

	/** 设置强化池 DataTable */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Upgrade")
	void SetUpgradePool(UDataTable* InTable) { UpgradePool = InTable; }

	/** 获取某张强化卡的当前叠加层数 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Upgrade")
	int32 GetUpgradeStacks(FName UpgradeID) const;

protected:
	/** 强化池 DataTable（DT_Upgrades） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Upgrade")
	TObjectPtr<UDataTable> UpgradePool;

	/** 每次抽取的候选数量 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Upgrade")
	int32 OptionsCount = 3;

private:
	/** 每张强化卡已叠加层数（用于判断是否已达上限） */
	TMap<FName, int32> UpgradeStacks;
};
