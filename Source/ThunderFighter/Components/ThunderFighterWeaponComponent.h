// ThunderFighter - 雷霆战机 WeaponComponent
// 管理玩家的射击逻辑、子弹生成和武器等级

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/UpgradeTypes.h"
#include "ThunderFighterWeaponComponent.generated.h"

class AProjectileBase;

/**
 * 武器强化修饰符。Roguelike 强化卡通过这些修饰符叠加到基础武器上。
 */
USTRUCT(BlueprintType)
struct FWeaponModifiers
{
	GENERATED_BODY()

	/** +弹道数（在基础武器模式上叠加） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 BonusBullets = 0;

	/** 伤害倍率（1.0 = 基础伤害） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DamageMultiplier = 1.0f;

	/** 射速倍率（1.0 = 基础射速） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRateMultiplier = 1.0f;

	/** 子弹可穿透敌人 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bPiercing = false;

	/** 子弹追踪最近敌人 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bHoming = false;

	/** 子弹命中后弹射次数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 BounceCount = 0;

	/** 侧翼僚机数量（每级两侧各 +1 个发射位） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 SideWingCount = 0;
};

/**
 * 附加到玩家 Pawn 上。管理武器等级、射速、弹幕生成模式和强化修饰符。
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UThunderFighterWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UThunderFighterWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 开始连续射击 */
	void StartFiring();

	/** 停止连续射击 */
	void StopFiring();

	/** 激活炸弹（清屏攻击） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Weapon")
	void ActivateBomb();

	/** 向玩家库存中添加炸弹 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Weapon")
	void AddBomb(int32 Count = 1);

	/** 武器等级提升 1 级 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Weapon")
	void UpgradeWeapon();

	/** 应用一个强化效果（Roguelike 强化卡调用） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Weapon|Upgrade")
	void ApplyModifier(EUpgradeEffect Effect, float Value);

	/** 获取当前强化修饰符 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Weapon|Upgrade")
	const FWeaponModifiers& GetModifiers() const { return Modifiers; }

	/** 获取当前武器等级 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Weapon")
	int32 GetWeaponLevel() const { return WeaponLevel; }

	/** 获取当前炸弹数量 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Weapon")
	int32 GetBombCount() const { return BombCount; }

protected:
	/** 在给定的局部偏移位置生成弹幕 */
	void FireProjectile(FVector LocalOffset);

	/** 根据当前武器等级模式射击 */
	void FirePattern();

	// ---- 可配置属性 ----

	/** 要生成的弹幕类型 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	TSubclassOf<AProjectileBase> ProjectileClass;

	/** 1 级的基础射速（每秒射击次数） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	float BaseFireRate = 5.0f;

	/** 当前武器等级（1-5） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	int32 WeaponLevel = 1;

	/** 最大武器等级 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	int32 MaxWeaponLevel = 5;

	/** 弹幕速度（单位/秒） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	float ProjectileSpeed = 2000.0f;

	/** 弹幕伤害 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	float ProjectileDamage = 10.0f;

	/** 可用炸弹数量 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon")
	int32 BombCount = 3;

	/** 1 级生成偏移（单发居中） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon|Patterns")
	TArray<FVector> PatternLevel1;

	/** 2 级生成偏移（双发） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon|Patterns")
	TArray<FVector> PatternLevel2;

	/** 3 级生成偏移（三连发） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon|Patterns")
	TArray<FVector> PatternLevel3;

	/** 4 级生成偏移（四连发） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon|Patterns")
	TArray<FVector> PatternLevel4;

	/** 5 级生成偏移（全扩散） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Weapon|Patterns")
	TArray<FVector> PatternLevel5;

private:
	/** 武器当前是否正在射击？ */
	bool bIsFiring = false;

	/** 射速时间累计器 */
	float FireTimer = 0.0f;

	/** 当前强化修饰符（Roguelike 叠加） */
	FWeaponModifiers Modifiers;
};
