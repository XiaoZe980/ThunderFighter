// ThunderFighter - 雷霆战机 HealthComponent
// 玩家和敌人通用的可复用生命值管理组件

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThunderFighterHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTaken, float, DamageAmount);

/**
 * 管理任意 Actor 的生命值、伤害和死亡逻辑的组件。
 * 附加到玩家战机、敌人或可破坏物体上使用。
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UThunderFighterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UThunderFighterHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	/** 对该 Actor 造成伤害 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void TakeDamage(float DamageAmount);

	/** 恢复指定数量的生命值 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void Heal(float HealAmount);

	/** 此 Actor 是否仍然存活？ */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	bool IsAlive() const { return CurrentHealth > 0.0f; }

	/** 获取当前生命值百分比（0.0 - 1.0） */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetHealthPercent() const { return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f; }

	/** 获取当前生命值 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	/** 获取最大生命值 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetMaxHealth() const { return MaxHealth; }

	/** 设置最大生命值并可选择重置当前生命值 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void SetMaxHealth(float NewMaxHealth, bool bResetCurrentHealth = true);

	/** 授予护盾值（先吸收伤害，护盾耗尽再扣血） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void GrantShield(float ShieldAmount);

	/** 获取当前护盾值 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetShield() const { return Shield; }

	/** 每秒回血（再生强化） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void SetRegeneration(float PerSecond);

	/** 获取当前每秒回血量 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetRegeneration() const { return RegenerationPerSecond; }

	// ---- 委托 ----

	/** 生命值归零时触发 */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Health")
	FOnHealthDepleted OnHealthDepleted;

	/** 生命值变化时触发 */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Health")
	FOnHealthChanged OnHealthChanged;

	/** 受到伤害时触发 */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Health")
	FOnDamageTaken OnDamageTaken;

protected:
	/** 最大生命值 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Health")
	float MaxHealth = 100.0f;

	/** 当前生命值 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Health")
	float CurrentHealth = 100.0f;

	/** 护盾值（先吸收伤害） */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Health")
	float Shield = 0.0f;

	/** 每秒回血量（再生强化，0 = 不回血） */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Health")
	float RegenerationPerSecond = 0.0f;

	/** 此 Actor 是否已死亡？（防止多次触发死亡事件） */
	bool bIsDead = false;
};
